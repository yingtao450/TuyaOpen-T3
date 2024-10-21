 /*============================================================================
 *                                                                            *
 * Copyright (C) by Tuya Inc                                                  *
 * All rights reserved                                                        *
 *                                                                            *
 =============================================================================*/


/*============================ INCLUDES ======================================*/
#include "tuya_cloud_types.h"
#include "tuya_error_code.h"
#include "cli.h"

#include "ethernetif.h"
#include "lwip/netifapi.h"
#include "lwip/etharp.h"
#include "lwip/prot/dhcp.h"
#include "lwip/dns.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "bk_misc.h"

#include "tkl_gpio.h"
#include "tkl_wired.h"
#include "tkl_thread.h"
#include "tkl_mutex.h"
#include "tkl_system.h"
#include "tkl_spi.h"
#include "tkl_output.h"
#include "tkl_memory.h"

#include "spi_eth_drv_ch390.h"
#include "tkl_wired.h"
#include "spi_eth_drv.h"
#ifdef SPI_LAN_LINK_SAFE_NET_CHK
#include "mqc_app.h"
#include "tuya_devos_utils.h"
#include "tuya_svc_netmgr.h"
#endif /* SPI_LAN_LINK_SAFE_NET_CHK */

#ifdef SPI_LAN_PKT_TRACE
#include "tal_log.h"
#endif /* SPI_LAN_PKT_TRACE */

#define INT_PIN  TUYA_GPIO_NUM_13
#define RST_PIN  TUYA_GPIO_NUM_12
#define CS_PIN   TUYA_GPIO_NUM_15
#define SCK_PIN  TUYA_GPIO_NUM_14
#define MISO_PIN TUYA_GPIO_NUM_17
#define MOSI_PIN TUYA_GPIO_NUM_16
#define WOL_PIN  TUYA_GPIO_NUM_17

#define SPI_ETHERNET_IFNAME0 'e'
#define SPI_ETHERNET_IFNAME1 'n'
#define CH390_THRAD_NAME "spi_eth_proc"

#define SPI_ETHERNET_FREQ_DEF 1024000
#define SPI_ETHERNET_THREAD_SIZE 2048
#define SPI_ETHERNET_THREAD_PRIORITY 5

static SemaphoreHandle_t spi_eth_sem = NULL;

#ifdef SPI_LAN_LOCK_SUPPORT
TKL_MUTEX_HANDLE spi_eth_lock = NULL;
#endif /* SPI_LAN_LOCK_SUPPORT */

struct netif *ch390_netif = NULL;

TKL_THREAD_HANDLE spi_eth_thread = NULL;

unsigned int interrupt_cnt = 0;
unsigned char last_interrupt_status = 0;
unsigned char spi_netif_link_status = 0; 

#ifdef CH390_TX_ERR_CHK_EN
static unsigned short trpa_value = 0;
static unsigned short mwr_value = 0;
unsigned int reset_tx_fifo_cnt = 0;
static unsigned char reset_tx_chk = 0;
unsigned char is_need_reset_tx_fifo = 0;
#endif /* CH390_TX_ERR_CHK_EN */

#ifdef SPI_LAN_LINK_SAFE_NET_CHK
#define CH390_SAFENET_CHK_RESET_THRES 1
unsigned char ch390_safenet_chk_cnt = 0;
unsigned char ch390_safenet_reset = 0;
os_timer_handle_t ch390_safenet_chk_timer = NULL;
#endif /* SPI_LAN_LINK_SAFE_NET_CHK */

TKL_WIRED_BASE_CFG_T spi_eth_cfg = {
    .int_gpio = INT_PIN,
};

extern TKL_WIRED_STATUS_CHANGE_CB spi_netif_link_chg_cb;

#ifdef SPI_LAN_PKT_TRACE
#define CH390_PKT_TRACE_BASIC 0x01
#define CH390_PKT_TRACE_DETIL 0x02
unsigned char drv_pkt_trace_en = CH390_PKT_TRACE_BASIC;
#endif /* SPI_LAN_PKT_TRACE */

#ifdef SPI_LAN_DBG_EN
typedef struct {
    unsigned int    tx_ok;
    unsigned int    tx_fail;
    unsigned int    rx_ok;
    unsigned int    rx_fail;
    unsigned int    rx_drop;
} spi_ethernet_txrx_stats_t;
spi_ethernet_txrx_stats_t ch390_txrx_stats;
void spi_eth_status_dump(void);
#endif /* SPI_LAN_DBG_EN */

void spi_ethernetif_input(struct netif *netif);
extern void *net_get_spi_eth_handle(void);
extern void *net_get_sta_handle(void);

#ifdef SPI_LAN_PKT_TRACE
void tuya_ethernetif_pkt_decode(unsigned char direction,  void *netif, void *pbuf);
#endif /* SPI_LAN_PKT_TRACE */

static void lan_chip_rst(uint8_t level)
{
#ifdef SPI_LAN_HARDWARE_RESET_EN
    tkl_gpio_write(RST_PIN, level);
#endif  
}

static void spi_lan_scs(uint8_t level)
{
    return;
}

static uint8_t spi_exchange_byte(uint8_t byte)
{
    int ret;
    uint8_t read_buf[32] = { 0 };
    uint8_t write_buf[32] = { 0 };

    write_buf[0] = byte;
    ret = tkl_spi_transfer(TUYA_SPI_NUM_0, write_buf, read_buf, 1);
    if (ret < 0) {
        SPI_LAN_DBG("%s: call tkl_spi_transfer failed(ret=%d)", __func__, ret);
    }
   
    return read_buf[0];
}

static void spi_lan_delay_us(uint32_t time)
{
    delay_us(time);
}

ch390_interface_t ch390_interface = {
    lan_chip_rst,
    spi_lan_delay_us,
    spi_lan_scs,
    spi_exchange_byte,
};

static int is_sta_link_actived(void)
{
    struct netif *sta_netif;

    sta_netif = net_get_sta_handle();
    if (   sta_netif && (sta_netif->flags & NETIF_FLAG_UP) 
        && sta_netif->ip_addr.addr && sta_netif->gw.addr 
        && sta_netif->netmask.addr) {
        return 1;
    }

    return 0;
}

#ifdef SPI_LAN_DBG_EN
void ch390_print_info()
{
    uint8_t ver;
    uint16_t vid, pid;
    uint8_t mac_addr[6] = { 0 };

    ver = ch390_get_revision();
    vid = ch390_get_vendor_id();
    pid = ch390_get_product_id();
    ch390_get_mac(mac_addr);
    SPI_LAN_DBG("%s: thernet adapter %04x%04x mac %02x:%02x:%02x:%02x:%02x:%02x ver %d",
        __func__, vid, pid, mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4],
        mac_addr[5], ver);
}
#endif /* SPI_LAN_DBG_EN */

static void ch390_interrupt_handler(void *args)
{
    int ret;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t irq_status = taskENTER_CRITICAL_FROM_ISR();

    ret = xSemaphoreGiveFromISR(spi_eth_sem, &xHigherPriorityTaskWoken);
    if (pdTRUE == xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
#ifdef SPI_LAN_DBG_TRACE
    if (pdFAIL == ret) {
        SPI_LAN_DBG("%s: call xSemaphoreGiveFromISR failed(ret=%d)", __func__, ret);
    }
#endif /* SPI_LAN_DBG_TRACE */
    interrupt_cnt++;
    taskEXIT_CRITICAL_FROM_ISR(irq_status);
}

static void spi_eth_sys_deinit()
{
    if (spi_eth_sem) {
        vSemaphoreDelete(spi_eth_sem);
        spi_eth_sem = NULL;
    }

 #ifdef SPI_LAN_LOCK_SUPPORT
    if (spi_eth_lock) {
        tkl_mutex_release(spi_eth_lock);
        spi_eth_lock = NULL;
    }
 #endif

    if (spi_eth_thread) {
        tkl_thread_release(spi_eth_thread);
        spi_eth_thread = NULL;
    }

#ifdef SPI_LAN_LINK_SAFE_NET_CHK
    if (ch390_safenet_chk_timer) {
        os_timer_stop(ch390_safenet_chk_timer);
        os_timer_delete(ch390_safenet_chk_timer);
        ch390_safenet_chk_timer = NULL;
    }
#endif /* SPI_LAN_LINK_SAFE_NET_CHK */
}

static void spi_eth_thread_handle(void *args)
{
    struct ethernetif *ether;
    TUYA_GPIO_LEVEL_E level;
     struct netif *sta_netif;

    uint8_t int_status;
    uint8_t link_status;

#ifdef CH390_TX_ERR_CHK_EN
    uint8_t trpal, trpah, mwrl, mwrh;
#endif

    ch390_netif = (struct netif *)net_get_spi_eth_handle();
    if (NULL == ch390_netif) {
        SPI_LAN_ERR("%s: start spi ethernet failed,"
                        "because of null netif", __func__);
        return;
    }
    
    while (xSemaphoreTake(spi_eth_sem, portMAX_DELAY)) {
        while (OPRT_OK == tkl_gpio_read(spi_eth_cfg.int_gpio, &level) && (level == TUYA_GPIO_LEVEL_HIGH)) {
        SPI_ETHERNET_LOCK_X(); 
        int_status = ch390_get_int_status();
        
        //SPI_LAN_DBG("%s: int_status 0x%x", __func__, int_status);
        if (int_status & ISR_LNKCHG) {
            spi_lan_delay_us(100000);
            ch390_write_phy(CH390_GPR, 0);
            while(ch390_read_phy(CH390_GPCR));
            link_status = ch390_get_link_status();
            if (link_status) {
                SPI_LAN_DBG("%s: netif link up link_status 0x%x", __func__, link_status);
                ch390_write_reg(CH390_ISR, ISR_LNKCHG);
                netifapi_netif_set_link_up(ch390_netif);
                netifapi_netif_set_up(ch390_netif);
                if (!is_sta_link_actived()) {
                    netifapi_netif_set_default(ch390_netif);
                }
                netifapi_dhcp_start(ch390_netif);
                if (spi_netif_link_chg_cb) {
                    spi_netif_link_chg_cb(TKL_WIRED_LINK_UP);
                }
                spi_netif_link_status = 1;
            } else {
                SPI_LAN_DBG("%s: netif link down link_status 0x%x", __func__, link_status);
                netifapi_dhcp_stop(ch390_netif);
                netifapi_netif_set_link_down(ch390_netif);
                netifapi_netif_set_down(ch390_netif);
                if (spi_netif_link_chg_cb) {
                    spi_netif_link_chg_cb(TKL_WIRED_LINK_DOWN);
                }
                                
                sta_netif = net_get_sta_handle();
                if (sta_netif) {
                    netifapi_netif_set_default(sta_netif);
                }
                spi_netif_link_status = 0;
            }
        }

        // Packet transmitted
        if (int_status & ISR_PT) {
            //SPI_LAN_DBG("%s: Packet transmitted", __func__);
#ifdef CH390_TX_ERR_CHK_EN
        trpal = ch390_read_reg(CH390_TRPAL);
        trpah = ch390_read_reg(CH390_TRPAH);
        mwrl = ch390_read_reg(CH390_MWRL);
        mwrh = ch390_read_reg(CH390_MWRH);
        
        trpa_value = trpal | (trpah << 8);
        mwr_value = mwrl | (mwrh << 8);
 
        if ((trpa_value == mwr_value) || (trpa_value <= (mwr_value + 3))) {
            reset_tx_chk = 0;
        } else {
            reset_tx_chk++; 
        }

       //SPI_LAN_DBG("%s: TRPAL=0x%x,TRPAH=0x%x,MWRL=0x%x,MWRH=0x%x,TRPA=0x%x,MWR=0x%x,reset_tx_chk=%d",
       //     __func__, trpal, trpah, mwrl, mwrh, trpa_value, mwr_value, reset_tx_chk);

        if (reset_tx_chk >= 1) {
            SPI_LAN_DBG("%s: Tx blocked of CH390, reset TX FIFO pointer"
            "(TRPAL=0x%x,TRPAH=0x%x,MWRL=0x%x,MWRH=0x%x,TRPA=0x%x,MWR=0x%x)",
                    __func__, trpal, trpah, mwrl, mwrh, trpa_value, mwr_value);
            reset_tx_chk = 0;
            is_need_reset_tx_fifo = 1;
        }
#endif /* CH390_TX_ERR_CHK_EN */    
        }

        // Receive packet
        if ((int_status & ISR_PR) || (int_status & ISR_ROS)) {
            ether = ch390_netif->state;
            if (NULL != ether) {
                do {
                    spi_ethernetif_input(ch390_netif);
                } while(ether->rx_len != 0);
            }  else {
                 SPI_LAN_ERR("%s: ethernetif state is null", __func__);
            }
        }
        
        if (int_status & ISR_ROO) {
            SPI_LAN_DBG("%s: Receive overflow counter overflow", __func__);
        }

        last_interrupt_status = int_status;
        SPI_ETHERNET_UNLOCK_X();
        } /* end while (OPRT_OK == tkl_gpio_read(spi_eth_cfg.int_gpio, &level) && (level == TUYA_GPIO_LEVEL_HIGH)) { */
        tkl_gpio_write(spi_eth_cfg.int_gpio, TUYA_GPIO_LEVEL_LOW);
    } /* end while(xQueueReceive(spi_ethernet_queue, message, portMAX_DELAY)) { */

    //Thread destory
    spi_eth_sys_deinit();
}

#ifdef SPI_LAN_LINK_SAFE_NET_CHK
void spi_eth_safenet_chk_timer(void* args)
{
    //SPI_LAN_DBG("%s: link_status %d int_cnt %u tx_ok %u tx_fail %u rx_ok %u rx_fail %u netmgr_status %d chk_cnt %d mqtt_stat %d active %d",
    //    __func__, spi_netif_link_status, interrupt_cnt, ch390_txrx_stats.tx_ok, ch390_txrx_stats.tx_fail,
    //    ch390_txrx_stats.rx_ok, ch390_txrx_stats.rx_fail, tuya_svc_netmgr_get_status(), ch390_safenet_chk_cnt, get_mqc_conn_stat(), get_gw_active());
	if (spi_netif_link_status && (ACTIVATED == get_gw_active()) && (FALSE == get_mqc_conn_stat())) {
        ch390_safenet_chk_cnt++;
        if ((!ch390_safenet_reset) && (ch390_safenet_chk_cnt >= CH390_SAFENET_CHK_RESET_THRES)) {
            SPI_LAN_DBG("%s: CH390 link was invalid, try to reset ch390 to recover it", __func__);
            ch390_software_reset();
            ch390_default_config();
            ch390_safenet_chk_cnt = 0;
            ch390_safenet_reset = 1;
        }
    } else {
        ch390_safenet_chk_cnt = 0;
        if (ch390_safenet_reset && spi_netif_link_status && (ACTIVATED == get_gw_active()) && (TRUE == get_mqc_conn_stat())) {
            ch390_safenet_reset = 0;
        }
    }
}
#endif /* SPI_LAN_LINK_SAFE_NET_CHK */

static int spi_eth_sys_init(void)
{
    int ret = -1;

    ch390_interface_register(ch390_interface);

    spi_eth_sem = xSemaphoreCreateBinary();
    if (NULL == spi_eth_sem) {
        SPI_LAN_ERR("%s: call xSemaphoreCreateBinary create spi eth sem failed", __func__);
        goto err_out;
    }

 #ifdef SPI_LAN_LOCK_SUPPORT 
    ret = tkl_mutex_create_init(&spi_eth_lock);
    if (ret < 0) {
        SPI_LAN_ERR("%s: call tkl_mutex_create_init  failed", __func__);
        goto err_out;
    }
  
    if (NULL == spi_eth_lock) {
        SPI_LAN_ERR("%s: create spi ehternet lockfailed", __func__);
        goto err_out;
    }
#endif /* SPI_LAN_LOCK_SUPPORT */

    ret = tkl_thread_create(&spi_eth_thread, CH390_THRAD_NAME,\
        SPI_ETHERNET_THREAD_SIZE, SPI_ETHERNET_THREAD_PRIORITY, spi_eth_thread_handle, NULL);
    if (ret < 0) {
        SPI_LAN_ERR("%s: call tkl_thread_create failed(ret=%d)", __func__, ret);
        goto err_out;
    }

    if (NULL == spi_eth_thread) {
        SPI_LAN_ERR("%s: create spi ethernet thead failed", __func__);
        ret = -1;
        goto err_out;
    }

#ifdef SPI_LAN_LINK_SAFE_NET_CHK
    ch390_safenet_chk_timer = os_timer_create("spi_ethernet_timer", 60000, 1, spi_eth_safenet_chk_timer, NULL);
    if (NULL == ch390_safenet_chk_timer) {
        SPI_LAN_ERR("%s: create spi ethernet link safenet check failed", __func__);
        ret = -1;
        goto err_out;
    }
#endif /* SPI_LAN_LINK_SAFE_NET_CHK */

    return ret;

err_out:
    if (spi_eth_sem) {
        vSemaphoreDelete(spi_eth_sem);
        spi_eth_sem = NULL;
    }

#ifdef SPI_LAN_LOCK_SUPPORT
    if (spi_eth_lock) {
        tkl_mutex_release(spi_eth_lock);
        spi_eth_lock = NULL;
    }
#endif
 
    if (spi_eth_thread) {
        tkl_thread_release(spi_eth_thread);
        spi_eth_thread = NULL;
    }
 
    return ret;
}

static int spi_eth_int_init(UINT32_T gpio_num, const TUYA_GPIO_IRQ_T *cfg)
{
    int ret;

    ret = tkl_gpio_irq_init(gpio_num, cfg);
    if (ret < 0) {
        SPI_LAN_ERR("%s: call tkl_gpio_irq_init failed(ret=%d)", __func__, ret);
        return ret;
    }

    return ret;
}

static int spi_eth_gpio_init(void)
{
    int ret;
    TUYA_GPIO_IRQ_T int_pin;
    TUYA_GPIO_BASE_CFG_T rst_pin;

    memset(&int_pin, 0, sizeof(int_pin));
    memset(&rst_pin, 0, sizeof(rst_pin));

    int_pin.mode = TUYA_GPIO_IRQ_RISE;
    int_pin.cb = ch390_interrupt_handler;
    int_pin.arg = NULL;

    ret = spi_eth_int_init(spi_eth_cfg.int_gpio, &int_pin);
    if (ret < 0) {
        SPI_LAN_ERR("%s: call tkl_gpio_irq_init init gpio %d failed", __func__, spi_eth_cfg.int_gpio);
        return -1;
    }

#ifdef SPI_LAN_HARDWARE_RESET_EN
    rst_pin.direct = TUYA_GPIO_OUTPUT;
    rst_pin.mode = TUYA_GPIO_PULLUP;
    rst_pin.level = TUYA_GPIO_LEVEL_LOW;
    ret = tkl_gpio_init(RST_PIN, &rst_pin);
    if (ret < 0) {
        SPI_LAN_ERR("%s: call tkl_gpio_init init gpio %d failed", __func__, RST_PIN);
        return -1;
    }

    tkl_gpio_write(RST_PIN, TUYA_GPIO_LEVEL_HIGH);
#endif /* SPI_LAN_HARDWARE_RESET_EN */
    return 0;
}

OPERATE_RET spi_eth_init(const TUYA_GPIO_NUM_E gpio_num)
{
    int ret;
    TUYA_SPI_BASE_CFG_T spi_cfg;

    if (spi_eth_sys_init() < 0) {
        return -1;
    }

    /* Init spi ehternet GPIOs */
    if (spi_eth_gpio_init() < 0) {
        spi_eth_sys_deinit();
        return -1;
    }

    /* Init SPI */
    memset(&spi_cfg, 0, sizeof(spi_cfg));
    spi_cfg.freq_hz = SPI_ETHERNET_FREQ_DEF;
    spi_cfg.bitorder = TUYA_SPI_ORDER_MSB2LSB;
    spi_cfg.databits = TUYA_SPI_DATA_BIT8;
    spi_cfg.role = TUYA_SPI_ROLE_MASTER;
    spi_cfg.mode = TUYA_SPI_MODE0;
    spi_cfg.spi_dma_flags = 1;
    spi_cfg.type = TUYA_SPI_AUTO_TYPE;

    ret = tkl_spi_init(TUYA_SPI_NUM_0, &spi_cfg);
    if (ret < 0) {
        SPI_LAN_ERR("%s: call spi_ini_cfg init spi failed", __func__);
        spi_eth_sys_deinit();
        return -1;
    }

#ifdef SPI_LAN_HARDWARE_RESET_EN
    /* Reset CH390 PHY */
    ch390_hardware_reset();
#else
    ch390_software_reset();
#endif

#ifdef SPI_LAN_DBG_EN
    //ch390_print_info();
#endif /* SPI_LAN_DBG_EN */

#ifdef SPI_LAN_LINK_SAFE_NET_CHK
    if (ch390_safenet_chk_timer) {
        ret = os_timer_start(ch390_safenet_chk_timer);
        if (ret < 0) {
            SPI_LAN_ERR("%s: call os_timer_start failed(ret=%d)", __func__, ret);
            spi_eth_sys_deinit();
            return -1;
        }
    }
#endif /* SPI_LAN_LINK_SAFE_NET_CHK */

    return 0;
}

//#if CONFIG_SPI_ETH
static void spi_eth_low_level_init(struct netif *netif)
{
    //init spi
    spi_eth_init(spi_eth_cfg.int_gpio);

    //struct ethernetif *ethernetif = netif->state;
    ch390_default_config();

    /* set MAC hardware address length */
    //netif->hwaddr_len = ETHARP_HWADDR_LEN;
    netif->hwaddr_len = 6;

    /* set MAC hardware address */
    ch390_get_mac(netif->hwaddr);
   
    /* maximum transfer unit */
//#ifdef SPI_LAN_PKT_FRAGMENT_EN
    netif->mtu = CH390_PKT_MAX; /* CH390_PKT_MAX */
//#else    
//    netif->mtu = 490; /* ECR660 SPI Support 512 */
//#endif /* SPI_LAN_PKT_FRAGMENT_EN */

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_ETHERNET;
    
    /* Do whatever else is needed to initialize interface. */
}

static err_t spi_eth_low_level_output(struct netif *netif, struct pbuf *p)
{
    struct pbuf *q;
#ifdef CH390_TX_FIFO_POINTER_EN    
    uint16_t tx_p = 0, tx_n = 0, tx_offset = 0;
#endif /* CH390_TX_FIFO_POINTER_EN */
#ifdef SPI_LAN_PKT_FRAGMENT_EN
    uint32_t pkt_size;
    uint8_t *ptr;
#endif /* SPI_LAN_PKT_FRAGMENT_EN */

    if (NULL == netif || NULL == p) {
        SPI_LAN_DBG("%s: input invalid param", __func__);
        return ERR_MEM;
    }

    SPI_ETHERNET_LOCK_X();

    #if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
    #endif

    for(q = p; q != NULL; q = q->next) {
        /* Send the data from the pbuf to the interface, one pbuf at a
           time. The size of the data in each pbuf is kept in the ->len
           variable. */
#ifdef SPI_LAN_PKT_TRACE /* packet trace here */
    if (drv_pkt_trace_en & CH390_PKT_TRACE_BASIC) {
        tuya_ethernetif_pkt_decode(1, netif, q);
    }
    if (drv_pkt_trace_en & CH390_PKT_TRACE_DETIL) {
        TAL_PR_HEXDUMP_NOTICE("send eth packet", q->payload, q->len);
    }
#endif /* SPI_LAN_PKT_TRACE */

#ifdef CH390_TX_ERR_CHK_EN
        if (is_need_reset_tx_fifo) {
            SPI_LAN_ERR("%s: need reset ch390 tx fifo before send packet", __func__);
#ifdef CH390_TX_ERR_CHK_EN_RESET            
            ch390_write_reg(CH390_MPTRCR, 0x02);  //Reset TX FIFO pointer
#endif           
            is_need_reset_tx_fifo = 0;
            reset_tx_fifo_cnt++;
        }
#endif /* CH390_TX_ERR_CHK_EN */

#ifdef CH390_TX_FIFO_POINTER_EN
    do {
#ifdef CH390_TX_FIFO_POINTER_DOUBLE_CHK        
            ch390_MWR();
#endif /* CH390_TX_FIFO_POINTER_DOUBLE_CHK */    
			tx_p = ch390_MWR();
#endif /* CH390_TX_FIFO_POINTER_EN */

#ifdef SPI_LAN_PKT_FRAGMENT_EN
            pkt_size = q->len;
            ptr = (uint8_t *)q->payload;
            if (pkt_size > SPI_LAN_PKT_MAX_LEN) {
                while (pkt_size) {
                    if (pkt_size > SPI_LAN_PKT_MAX_LEN) {
                        ch390_write_mem(ptr, SPI_LAN_PKT_MAX_LEN);
                        ptr += SPI_LAN_PKT_MAX_LEN;
                        pkt_size -= SPI_LAN_PKT_MAX_LEN;
                    } else {
                        ch390_write_mem(ptr, pkt_size);
                        pkt_size -= pkt_size;
                    }
                }
            } else {
                ch390_write_mem(q->payload, q->len);
            }
#else
            ch390_write_mem(q->payload, q->len);
#endif  /* SPI_LAN_PKT_FRAGMENT_EN */

#ifdef CH390_TX_FIFO_POINTER_EN
#ifdef CH390_TX_FIFO_POINTER_DOUBLE_CHK
			ch390_MWR();
#endif /* CH390_TX_FIFO_POINTER_DOUBLE_CHK */      
            tx_n = ch390_MWR();
            if (tx_n > tx_p) {
                tx_offset = tx_n - tx_p;
            } else {
                tx_offset = 0xc00 - tx_p + tx_n;
            }
             
            if (tx_offset != q->len) {
#ifdef CH390_TX_FIFO_POINTER_EN_RESET                
                ch390_write_reg(CH390_MPTRCR, 0x02);  //Reset TX FIFO pointer
#endif                
                SPI_LAN_ERR("%s: send packet failed, ch390 TX invalid(tx_p=0x%x,tx_n=0x%x,q_len=%d,tx_offset=%d)",
                    __func__, tx_p, tx_n, q->len, tx_offset);
#ifdef SPI_LAN_DBG_EN                    
                ch390_txrx_stats.tx_fail++;
#endif                
            }
        } while (tx_offset != q->len);
#endif /* CH390_TX_FIFO_POINTER_EN */     
    } /* end for(q = p; q != NULL; q = q->next) { */

    #if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
    #endif
 
    // Wait until last transmit complete
    while(ch390_read_reg(CH390_TCR) & TCR_TXREQ) {
#ifdef SPI_LAN_DBG_TRACE        
        SPI_LAN_DBG("%s: waitting for ch390 tx req flag", __func__);
        delay_us(100);
#endif        
    }
    
    // Set current packet length
    ch390_write_reg(CH390_TXPLL, p->tot_len & 0xff);
    ch390_write_reg(CH390_TXPLH, (p->tot_len >> 8) & 0xff);
    // Issue transmit request
    ch390_send_request();
    LINK_STATS_INC(link.xmit);
#ifdef SPI_LAN_DBG_EN    
    ch390_txrx_stats.tx_ok++;
#endif

    SPI_ETHERNET_UNLOCK_X();
    return ERR_OK;
}

static struct pbuf *spi_eth_low_level_input(struct netif *netif)
{
    struct ethernetif *ethernetif;
    struct pbuf *p, *q;
    u16_t len;
    u8_t rx_ready;
    u8_t ReceiveData[4];
#ifdef SPI_LAN_PKT_FRAGMENT_EN
    uint32_t pkt_size;
    uint8_t *ptr;
#endif /* SPI_LAN_PKT_FRAGMENT_EN */

    if (NULL == netif) {
        SPI_LAN_ERR("%s: netif is null", __func__);
        return NULL;
    }

    ethernetif = netif->state;
    if (NULL == ethernetif) {
        SPI_LAN_ERR("%s: ethernetif is null", __func__);
        return NULL;
    }

    ch390_read_reg(CH390_MRCMDX);
    rx_ready = ch390_read_reg(CH390_MRCMDX);
    if (rx_ready & CH390_PKT_ERR)
    {
        // Reset RX FIFO pointer
        ch390_rx_reset();
        LINK_STATS_INC(link.drop);
        ethernetif->rx_len = 0;
#ifdef SPI_LAN_DBG_EN        
        ch390_txrx_stats.rx_fail++;
#endif        
        SPI_LAN_ERR("%s: receive packet err(rx_ready=0x%x)", __func__, rx_ready);
        return NULL;
    }

    if (!(rx_ready & CH390_PKT_RDY))
    {
        ethernetif->rx_len = 0;
        if (last_interrupt_status & ISR_ROS) {
            ch390_rx_reset();
            SPI_LAN_DBG("%s: ====receive packet RX not ready(rx_ready=0x%x)", __func__, rx_ready);
        }

//#ifdef SPI_LAN_DBG_TRACE       
        //SPI_LAN_DBG("%s: receive packet RX not ready(rx_ready=0x%x)", __func__, rx_ready);
//#endif
        return NULL;
    }

    ch390_read_mem(ReceiveData, 4);
    ethernetif->rx_status = ReceiveData[1];
    ethernetif->rx_len = ReceiveData[2] | (ReceiveData[3] << 8);

    /* Obtain the size of the packet and put it into the "len" variable. */
    len = ethernetif->rx_len;

    if ((ethernetif->rx_status & 0x3f) || (0 == len) || (len > CH390_PKT_MAX)) {
        SPI_LAN_ERR("%s: invaild input packet(status=0x%x,len=%d)", __func__, ethernetif->rx_status, len);
        ch390_rx_reset();
        ethernetif->rx_len = 0;
#ifdef SPI_LAN_DBG_EN        
        ch390_txrx_stats.rx_fail++;
#endif        
        return NULL;
    }

    #if ETH_PAD_SIZE
    len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
    #endif

    /* We allocate a pbuf chain of pbufs from the pool. */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
    if (p != NULL) {
        #if ETH_PAD_SIZE
        pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
        #endif

        /* We iterate over the pbuf chain until we have read the entire
         * packet into the pbuf. */
    for(q = p; q != NULL; q = q->next) {
        /* Read enough bytes to fill this pbuf in the chain. The
        * available data in the pbuf is given by the q->len
        * variable.
        * This does not necessarily have to be a memcpy, you can also preallocate
        * pbufs for a DMA-enabled MAC and after receiving truncate it to the
        * actually received size. In this case, ensure the tot_len member of the
        * pbuf is the sum of the chained pbuf len members.
        */
#ifdef SPI_LAN_PKT_FRAGMENT_EN
        pkt_size = q->len;
        ptr = (uint8_t *)q->payload;

        if (pkt_size > SPI_LAN_PKT_MAX_LEN) {
            while (pkt_size) {
                if (pkt_size > SPI_LAN_PKT_MAX_LEN) {
                    ch390_read_mem(ptr, SPI_LAN_PKT_MAX_LEN);
                    ptr += SPI_LAN_PKT_MAX_LEN;
                    pkt_size -= SPI_LAN_PKT_MAX_LEN;
                } else {
                    ch390_read_mem(ptr, pkt_size);
                    pkt_size -= pkt_size;
                }
            }
        } else {
            ch390_read_mem(q->payload, q->len);
        }
#else
        ch390_read_mem(q->payload, q->len);
#endif  /* SPI_LAN_PKT_FRAGMENT_EN */

#ifdef SPI_LAN_PKT_TRACE
        if (drv_pkt_trace_en & CH390_PKT_TRACE_BASIC) {
            tuya_ethernetif_pkt_decode(0, netif, q);
        }
        if (drv_pkt_trace_en & CH390_PKT_TRACE_DETIL) {
            TAL_PR_HEXDUMP_NOTICE("rcv eth packet", q->payload, q->len);
        }
#endif /* SPI_LAN_PKT_TRACE */
    } /* end for(q = p; q != NULL; q = q->next) { */
        #if ETH_PAD_SIZE
        pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
        #endif

#ifdef SPI_LAN_DBG_EN
        ch390_txrx_stats.rx_ok++;
#endif        
        LINK_STATS_INC(link.recv);
    } else {
        ch390_drop_packet(len);
        SPI_LAN_LOG("%s: failed to alloc pbuf drop the "
                "packet(rx_ready=0x%x,status=0x%x,len=%d)",
                __func__, rx_ready, ethernetif->rx_status, ethernetif->rx_len);
        LINK_STATS_INC(link.memerr);
        LINK_STATS_INC(link.drop);
#ifdef SPI_LAN_DBG_EN        
        ch390_txrx_stats.rx_drop++;
#endif        
    } /* p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL); */

    return p;
}

void spi_ethernetif_input(struct netif *netif)
{
    struct eth_hdr *ethhdr;
    struct pbuf *p;

    if (NULL == netif) {
        SPI_LAN_ERR("%s: netif is null", __func__);
        return;
    }

    /* move received packet into a new pbuf */
    p = spi_eth_low_level_input(netif);
    /* no packet could be read, silently ignore this */
    if (p == NULL) {
        return;
    }
    /* points to packet payload, which starts with an Ethernet header */
    ethhdr = p->payload;

    switch (htons(ethhdr->type)) {
    /* IP or ARP packet? */
    case ETHTYPE_IP:
    case ETHTYPE_ARP:
#if PPPOE_SUPPORT
    /* PPPoE packet? */
    case ETHTYPE_PPPOEDISC:
    case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
        /* full packet send to tcpip_thread to process */
        if (netif->input(p, netif) != ERR_OK) {
            SPI_LAN_ERR("%s: call input packet failed", __func__);
            pbuf_free(p);
            p = NULL;
        }
        break;

    default:
        pbuf_free(p);
        p = NULL;
        break;
  }
}

err_t spi_ethernetif_init(struct netif *netif)
{
    struct ethernetif *ethernetif;

    if (NULL == netif) {
        SPI_LAN_ERR("%s: netif is null", __func__);
        return ERR_MEM;
    }

    ethernetif = (struct ethernetif *)mem_malloc(sizeof(struct ethernetif));
    if (ethernetif == NULL) {
        LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
        SPI_LAN_ERR("%s: memory malloc failed", __func__);
        return ERR_MEM;
    }

    memset(ethernetif, 0, sizeof(struct ethernetif));

#ifdef SPI_LAN_DBG_EN   
    memset(&ch390_txrx_stats, 0, sizeof(ch390_txrx_stats));
#endif

    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    //NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 100);

    netif->state = ethernetif;
    netif->name[0] = SPI_ETHERNET_IFNAME0;
    netif->name[1] = SPI_ETHERNET_IFNAME1;

#if LWIP_NETIF_HOSTNAME
    netif->hostname = "tuyasmart-en";
#endif /* LWIP_NETIF_HOSTNAME */

    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    netif->output = etharp_output;
    netif->linkoutput = spi_eth_low_level_output;

    /* initialize the hardware */
    spi_eth_low_level_init(netif);

    /* enable spi ehernet interruput */
    if (tkl_gpio_irq_enable(spi_eth_cfg.int_gpio) < 0) {
        SPI_LAN_ERR("%s: call hal_gpio_intr_enable "
                "enable intr failed", __func__);
        return ERR_MEM;
    }

    return ERR_OK;
}
//#endif

#ifdef SPI_LAN_DBG_EN
void spi_eth_status_dump(void)
{
    NW_IP_S ip;
    TKL_WIRED_STAT_E link_status;
    int i;
    const ip_addr_t * dns_srv;

    SPI_ETHERNET_LOCK_X();
    ch390_reg_dump();
    SPI_ETHERNET_UNLOCK_X();

    SPI_LAN_LOG("%s: interrupt counter: %u", __func__, interrupt_cnt);
    SPI_LAN_LOG("%s: last interrupt status: 0x%x", __func__, last_interrupt_status);
    SPI_LAN_LOG("%s: Number of Tx packets success: %u", __func__, ch390_txrx_stats.tx_ok);
    SPI_LAN_LOG("%s: Number of Tx packets fail: %u", __func__, ch390_txrx_stats.tx_fail);
    SPI_LAN_LOG("%s: Number of Rx packets success: %u", __func__, ch390_txrx_stats.rx_ok);
    SPI_LAN_LOG("%s: Number of Rx packets fail: %u", __func__, ch390_txrx_stats.rx_fail);
    SPI_LAN_LOG("%s: Number of Rx packets drop: %u", __func__, ch390_txrx_stats.rx_drop);
#ifdef CH390_TX_ERR_CHK_EN
    SPI_LAN_LOG("%s: Number of reset Tx FIFO pointer: %u", __func__, reset_tx_fifo_cnt);
#endif /* CH390_TX_ERR_CHK_EN */ 

    tkl_wired_get_ip(&ip);
    tkl_wired_get_status(&link_status);
    SPI_LAN_DBG("%s: get wired ip %s mask %s gw %s",
        __func__, ip.ip, ip.mask, ip.gw);
    for (i = 0; i < DNS_MAX_SERVERS; i++) {
            dns_srv = dns_getserver(i);
            if (dns_srv && dns_srv->addr) {
                SPI_LAN_DBG("%s: DNS server %i ip %d.%d.%d.%d",
                    __func__, i, dns_srv->addr & 0xFF, (dns_srv->addr >> 8) & 0xFF, (dns_srv->addr >> 16) & 0xFF, (dns_srv->addr >> 24) & 0xFF);
            }
    } 
}
#endif

/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "common/bk_include.h"

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#ifdef CONFIG_IPV6
#include <lwip/ethip6.h>
#endif

#include "wlanif.h"

#include <stdio.h>
#include <string.h>

#include "netif/etharp.h"

#include "lwip_netif_address.h"

#include "bk_drv_model.h"
#include <os/mem.h>

#include <os/os.h>
// Modified by TUYA Start
/* Define those to better describe your network interface. */
#define IFNAME0 's'
#define IFNAME1 'a'
// Modified by TUYA End
#include "bk_uart.h"
#include "bk_wifi.h"
#include "bk_private/bk_wifi.h"

//TODO should use registered callback here!!!
extern int ke_l2_packet_tx(unsigned char *buf, int len, int flag);
extern int bmsg_tx_sender(struct pbuf *p, uint32_t vif_idx);
#if CONFIG_WIFI6_CODE_STACK
extern int bmsg_special_tx_sender(struct pbuf *p, uint32_t vif_idx);
#endif
/* Forward declarations. */
void ethernetif_input(int iface, struct pbuf *p);

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */

const char wlan_name[][6] =
{
    "wlan0\0",
    "wlan1\0",
    "wlan2\0",
    "wlan3\0",
};
static void low_level_init(struct netif *netif)
{
    void *vif = netif->state;
    u8 *macptr = wifi_netif_vif_to_mac(vif);
	int vif_index = wifi_netif_vif_to_vifid(vif);

#if LWIP_NETIF_HOSTNAME
// Modified by TUYA Start
    if(NULL == netif->hostname) {
        /* Initialize interface hostname */
        netif->hostname = (char*)&wlan_name[vif_index];
    }
// Modified by TUYA End
#endif /* LWIP_NETIF_HOSTNAME */

    /* set MAC hardware address length */
    LWIP_LOGD("enter low level!\r\n");
    LWIP_LOGI("mac %2x:%2x:%2x:%2x:%2x:%2x\r\n", macptr[0], macptr[1], macptr[2],
                 macptr[3], macptr[4], macptr[5]);

    netif->hwaddr_len = ETHARP_HWADDR_LEN;
    os_memcpy(netif->hwaddr, macptr, ETHARP_HWADDR_LEN);
    /* maximum transfer unit */
    netif->mtu = 1500;
    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
 #ifdef LWIP_IGMP
    netif->flags |= NETIF_FLAG_IGMP;
 #endif
 #if defined (LWIP_IPV6_MLD) && (LWIP_IPV6_MLD == 1)
    netif->flags |= NETIF_FLAG_MLD6;
 #endif
    LWIP_LOGD("leave low level!\r\n");
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	int ret;
	err_t err = ERR_OK;
	uint8_t vif_idx = wifi_netif_vif_to_vifid(netif->state);

	// Sanity check
	if (vif_idx == 0xff)
		return ERR_ARG;

#if CONFIG_WIFI6_CODE_STACK
	//LWIP_LOGI("output:%x\r\n", p);
	extern bool special_arp_flag;
	if(special_arp_flag)
	{
		ret = bmsg_special_tx_sender(p, (uint32_t)vif_idx);
		special_arp_flag = false;
	}
	else
#endif
	{
		ret = bmsg_tx_sender(p, (uint32_t)vif_idx);
	}
	if(0 != ret)
	{
		err = ERR_TIMEOUT;
	}

    return err;
}

static inline int is_broadcast_mac_addr(const u8 *a)
{
	return (a[0] & a[1] & a[2] & a[3] & a[4] & a[5]) == 0xff;
}

static inline int is_zero_ether_addr(const u8 *a)
{
	return !(a[0] | a[1] | a[2] | a[3] | a[4] | a[5]);
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void
ethernetif_input(int iface, struct pbuf *p)
{
    struct eth_hdr *ethhdr;
	struct netif *netif;
	void *vif;

	if (p->len <= SIZEOF_ETH_HDR) {
		pbuf_free(p);
		return;
	}

	vif = wifi_netif_vifid_to_vif(iface);
	netif = (struct netif *)wifi_netif_get_vif_private_data(vif);
	if(!netif) {
        //LWIP_LOGI("ethernetif_input no netif found %d\r\n", iface);
        pbuf_free(p);
        p = NULL;
        return;
    }

    /* points to packet payload, which starts with an Ethernet header */
    ethhdr = p->payload;

    if( (memcmp(netif->hwaddr,ethhdr->src.addr,NETIF_MAX_HWADDR_LEN)==0) && (htons(ethhdr->type) !=ETHTYPE_ARP) )
    {
        LWIP_DEBUGF(ETHARP_DEBUG ,("ethernet_input frame is my send,drop it\r\n"));
        pbuf_free(p);
        return;
    }

	/* need to forward*/
	if (wifi_netif_vif_to_netif_type(vif) == NETIF_IF_AP) {
		if (((!is_broadcast_mac_addr(ethhdr->dest.addr) && !is_zero_ether_addr(ethhdr->dest.addr) &&
			(memcmp(netif->hwaddr,ethhdr->dest.addr,NETIF_MAX_HWADDR_LEN) != 0))) ||
			(is_broadcast_mac_addr(ethhdr->dest.addr))) {
				struct pbuf *q;
				q = pbuf_clone(PBUF_RAW_TX, PBUF_RAM, p);
				if (q != NULL) {
					low_level_output(netif, q);
					pbuf_free(q);
				} else
					LWIP_LOGI("alloc pbuf failed, dont forward\r\n");
		}
	}

    switch (htons(ethhdr->type))
    {
        /* IP or ARP packet? */
    case ETHTYPE_IP:
    case ETHTYPE_ARP:
#ifdef CONFIG_IPV6
    case ETHTYPE_IPV6:
	wlan_set_multicast_flag_internal();
#endif
#if PPPOE_SUPPORT
        /* PPPoE packet? */
    case ETHTYPE_PPPOEDISC:
    case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
        /* full packet send to tcpip_thread to process */
        if (netif->input(p, netif) != ERR_OK)    // ethernet_input
        {
            LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\r\n"));
            pbuf_free(p);
            p = NULL;
        }
        break;

    case ETHTYPE_EAPOL:
	 	ke_l2_packet_tx(p->payload, p->len, iface);
		pbuf_free(p);
		p = NULL;
        break;

    default:
        pbuf_free(p);
        p = NULL;
        break;
    }

}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
wlanif_init(struct netif *netif)
{
    LWIP_ASSERT("netif != NULL", (netif != NULL));

    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 10000000);

    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    netif->output = etharp_output;
    netif->linkoutput = low_level_output;
#ifdef CONFIG_IPV6
    netif->output_ip6 = ethip6_output;
#endif

    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netifapi_netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t lwip_netif_init(struct netif *netif)
{
	LWIP_ASSERT("netif != NULL", (netif != NULL));

	/*
	 * Initialize the snmp variables and counters inside the struct netif.
	 * The last argument should be replaced with your link speed, in units
	 * of bits per second.
	 */
	NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, 10000000);

	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;
	/* We directly use etharp_output() here to save a function call.
	 * You can instead declare your own function an call etharp_output()
	 * from it if you have to do some checks before sending (e.g. if link
	 * is available...) */
	netif->output = etharp_output;
	netif->linkoutput = low_level_output;
#ifdef CONFIG_IPV6
	netif->output_ip6 = ethip6_output;
#endif

	/* initialize the hardware */
	low_level_init(netif);
	return ERR_OK;
}

err_t lwip_netif_uap_init(struct netif *netif)
{
	LWIP_ASSERT("netif != NULL", (netif != NULL));

	//netif->state = NULL;
	netif->name[0] = 'u';
	netif->name[1] = 'a';
	/* We directly use etharp_output() here to save a function call.
	 * You can instead declare your own function an call etharp_output()
	 * from it if you have to do some checks before sending (e.g. if link
	 * is available...) */
	netif->output = etharp_output;
	netif->linkoutput = low_level_output;

	/* initialize the hardware */
	low_level_init(netif);

	return ERR_OK;
}

// eof

#include "cli.h"
#include "modules/pm.h"
#include "modules/wifi.h"
#include "tuya_cloud_types.h"
#include "tkl_memory.h"
#include "tkl_system.h"
#include <driver/pwm.h>
#include "bk_rtos_debug.h"
#include "bk_phy.h"

extern size_t xPortGetFreeHeapSize( void );
extern void bk_printf(const char *fmt, ...);

static void cli_rf_set_cali_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    bk_printf("set rf calibration flag begin\r\n");

    char *arg[5];
    arg[0] = "txevm";
    arg[1] = "-e";
    arg[2] = "2\r\n";
    tx_evm_cmd_test(pcWriteBuffer, xWriteBufferLen, 3, arg);
    tkl_system_sleep(200);

    arg[0] = "txevm";
    arg[1] = "-g";
    arg[2] = "8\r\n";
    tx_evm_cmd_test(pcWriteBuffer, xWriteBufferLen, 3, arg);
    tkl_system_sleep(200);

    arg[0] = "txevm";
    arg[1] = "-g";
    arg[2] = "0\r\n";
    tx_evm_cmd_test(pcWriteBuffer, xWriteBufferLen, 3, arg);
    tkl_system_sleep(200);

    arg[0] = "txevm";
    arg[1] = "-e";
    arg[2] = "1\r\n";
    tx_evm_cmd_test(pcWriteBuffer, xWriteBufferLen, 3, arg);
    tkl_system_sleep(200);

    arg[0] = "txevm";
    arg[1] = "-s";
    arg[2] = "11";
    arg[3] = "1";
    arg[4] = "20\r\n";
    tx_evm_cmd_test(pcWriteBuffer, xWriteBufferLen, 5, arg);
    tkl_system_sleep(200);

    arg[0] = "txevm";
    arg[1] = "-e";
    arg[2] = "2\r\n";
    tx_evm_cmd_test(pcWriteBuffer, xWriteBufferLen, 3, arg);
    tkl_system_sleep(200);

    arg[0] = "txevm";
    arg[1] = "-e";
    arg[2] = "4";
    arg[3] = "1\r\n";
    tx_evm_cmd_test(pcWriteBuffer, xWriteBufferLen, 4, arg);
    tkl_system_sleep(200);

    arg[0] = "txevm";
    arg[1] = "-g";
    arg[2] = "8\r\n";
    tx_evm_cmd_test(pcWriteBuffer, xWriteBufferLen, 3, arg);
    tkl_system_sleep(200);

    bk_printf("set rf calibration flag end\r\n");
}

static void cli_wifi_set_interval_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    uint8_t interval = 0;
    int ret = 0;
    char *msg = NULL;

    if (argc < 2) {
        bk_printf("invalid argc num\r\n");
        goto error;
    }

    interval = (uint8_t)os_strtoul(argv[1], NULL, 10);

    bk_printf("\r\nset wifi dtim %d\r\n", interval);
    ret = bk_wifi_send_listen_interval_req(interval);

    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP, 1, 0);

    if (!ret) {
        bk_printf("set_interval ok");
        msg = WIFI_CMD_RSP_SUCCEED;
        os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
        return;
    }
    else {
        bk_printf("set_interval failed");
        goto error;
    }

error:
    msg = WIFI_CMD_RSP_ERROR;
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
    return;
}

#if CONFIG_FREERTOS_V10
extern void port_check_isr_stack(void);
#endif // CONFIG_FREERTOS_V10
static void cli_task_cpuload_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    if (argc > 1) {
        for (int i = 0; i < argc; i++) {
            switch (argv[i][0]) {
                case 't':
                    rtos_dump_backtrace();
                    break;
#if CONFIG_FREERTOS_V10
                case 's':
                    port_check_isr_stack();
                    break;
#endif // CONFIG_FREERTOS_V10
#if CONFIG_FREERTOS && CONFIG_MEM_DEBUG
                case 'm':
                    os_dump_memory_stats(0, 0, NULL);
                    break;
#endif // CONFIG_FREERTOS && CONFIG_MEM_DEBUG
                default:
                    bk_printf("unknown param: %s\r\n", argv[i]);
                    break;
            }
        }

    }

    rtos_dump_task_list();

#if CONFIG_FREERTOS
    rtos_dump_task_runtime_stats();
#endif // CONFIG_FREERTOS

    bk_printf("left heap: %d\r\n", xPortGetFreeHeapSize());

    return;
}

#include "driver/pwm_types.h"
struct pwm_test_s {
    uint32_t ch;
    uint32_t is_running;
    uint32_t is_init;
    uint32_t freq;
    uint32_t duty;
    pwm_init_config_t conf;
};
static struct pwm_test_s *pconf = NULL;
static void __pwm_usage(void)
{
    bk_printf("usage: xpwm set [chan] [freq] [duty]\r\n");
    bk_printf("       xpwm start/stop [chan]\r\n");
    bk_printf("       xpwm show/free/help\r\n");
    bk_printf("chan rank: [18,19,22,24,25]\r\n");
    bk_printf("freq: 1 - 10000 Hz\r\n");
    bk_printf("duty: 0 - 100 %%\r\n");
    bk_printf("default chan 18, freq 1000, duty f*0.25\r\n");
}
static int __get_free_pwm(uint32_t ch)
{
    int i;
    // first find if channel in used
    for (i = 0; i < 5; i++) {
        if (pconf[i].ch == ch) {
            return i;
        }
    }
    // new channel, record
    for (i = 0; i < 5; i++) {
        if (pconf[i].ch == 0) {
            return i;
        }
    }

    return 0xff;
}
static inline int __chan_check(uint32_t ch)
{
    switch(ch) {
        case 18:
        case 19:
        case 22:
        case 24:
        case 25:
            return 1;
        default:
            return 0;
    }
}
static inline int __freq_check(uint32_t freq)
{
    return (freq < 1 || freq > 10000)? 0: 1;
}
static inline int __duty_check(uint32_t duty)
{
    return (duty > 100)? 0: 1;
}
static inline void __chan_dump(struct pwm_test_s *p, char *tag)
{
    if (tag != NULL)
        bk_printf("ch: %d, freq: %d, duty: %d%%, period: %d, htime: %d, %s\r\n",
                p->ch, p->freq, p->duty, p->conf.period_cycle, p->conf.duty_cycle, tag);
    else
        bk_printf("ch: %d, freq: %d, duty: %d%%, period: %d, htime: %d\r\n",
                p->ch, p->freq, p->duty, p->conf.period_cycle, p->conf.duty_cycle);
}
#define __PWM_FREQ2PERIOD(x) ((unsigned int)((1000000.0/x)))
static inline pwm_chan_t __pwm_channel_remap(uint32_t ch)
{
    switch(ch) {
        case 18:
            return 0;
        case 19:
            return 1;
        case 22:
            return 2;
        case 24:
            return 4;
        case 25:
            return 5;
        default:
            return 0;
    }
}
static void cli_pwm_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    if (argc < 2 || argc > 5) {
        __pwm_usage();
        return;
    }

#if 0
    bk_printf("argc :%d\r\n", argc);
    for (int i = 0; i < argc; i++) {
        bk_printf("argv[%d]: %s\r\n", i, argv[i]);
    }
#endif

    struct pwm_test_s pwm;
    memset(&pwm, 0, sizeof(struct pwm_test_s));

    if (pconf == NULL) {
        pconf = (struct pwm_test_s *)tkl_system_malloc(5 * sizeof(struct pwm_test_s));
        if (pconf == NULL) {
            bk_printf("malloc for pwm test failed\r\n");
            return;
        }
        memset(pconf, 0, 5 * sizeof(struct pwm_test_s));
        for (int i = 0; i < 5; i++) {
            pconf[i].duty = 25;
            pconf[i].conf.period_cycle = 1000;
            pconf[i].conf.duty_cycle = 25;
        }
    }

    if (os_strcmp(argv[1], "set") == 0) {
        // default
        pwm.ch = 18;
        pwm.freq = 1000;
        pwm.duty = 25;
        pwm.conf.period_cycle = __PWM_FREQ2PERIOD(1000);
        pwm.conf.duty_cycle = pwm.conf.period_cycle / 4;

        switch (argc) {
            case 5: {    // duty
                pwm.duty = os_strtoul(argv[4], NULL, 10);
                if (pwm.duty > 100) {
                    bk_printf("duty set 100%\r\n");
                    pwm.duty = 100;
                }
            }
            case 4: {     // freq
                uint32_t freq = os_strtoul(argv[3], NULL, 10);
                if (!__freq_check(freq)) {
                    bk_printf("freq error: %d\r\n", freq);
                    __pwm_usage();
                    return;
                }
                pwm.freq = freq;
                pwm.conf.period_cycle = __PWM_FREQ2PERIOD(freq);
                if (argc == 4) { // set default
                    pwm.conf.duty_cycle = pwm.conf.period_cycle / 4;
                }
            }
            case 3: {     // channel
                uint32_t chan = os_strtoul(argv[2], NULL, 10);
                if (!__chan_check(chan)) {
                    bk_printf("no such channel: %d\r\n", chan);
                    __pwm_usage();
                    return;
                }
                pwm.ch = chan;
                if (argc == 3) { // set default
                    pwm.conf.period_cycle = 1000;
                    pwm.conf.duty_cycle = 250;
                }
            }
            default:
                break;
        }

        pwm.conf.duty_cycle = (pwm.conf.period_cycle * pwm.duty) / 100;
        int id = __get_free_pwm(pwm.ch);
        memcpy(&pconf[id], &pwm, sizeof(struct pwm_test_s));
        __chan_dump(&pconf[id], NULL);
    } else if (os_strcmp(argv[1], "start") == 0) {
        if (argc == 2) {
            bk_printf("all start\r\n");
            for (int i = 0; i < 5; i++) {
                if (pconf[i].ch != 0 && pconf[i].is_running != 1) {
                    if (!pconf[i].is_init) {
                        BK_LOG_ON_ERR(bk_pwm_init(__pwm_channel_remap(pconf[i].ch), &pconf[i].conf));
                        pconf[i].is_init = 1;
                    }
                    BK_LOG_ON_ERR(bk_pwm_start(__pwm_channel_remap(pconf[i].ch)));
                    pconf[i].is_running = 1;
                    __chan_dump(&pconf[i], "start");
                }
            }
        } else if (argc >= 3) {
            uint32_t chan = os_strtoul(argv[2], NULL, 10);
            if (!__chan_check(chan)) {
                bk_printf("no such channel: %d\r\n", chan);
                __pwm_usage();
                return;
            }
            int id = __get_free_pwm(chan);
            if (!pconf[id].is_init) {
                BK_LOG_ON_ERR(bk_pwm_init(__pwm_channel_remap(pconf[id].ch), &pconf[id].conf));
                pconf[id].is_init = 1;
            }
            BK_LOG_ON_ERR(bk_pwm_start(__pwm_channel_remap(pconf[id].ch)));
            pconf[id].is_running = 1;
            __chan_dump(&pconf[id], "start");
        }
    } else if (os_strcmp(argv[1], "stop") == 0) {
        if (argc == 2) {
            bk_printf("all stop\r\n");
            for (int i = 0; i < 5; i++) {
                if (pconf[i].ch != 0 && pconf[i].is_running == 1) {
                    BK_LOG_ON_ERR(bk_pwm_stop(__pwm_channel_remap(pconf[i].ch)));
                    pconf[i].is_running = 0;
                    __chan_dump(&pconf[i], "stop");
                }
            }
        } else if (argc >= 3) {
            uint32_t chan = os_strtoul(argv[2], NULL, 10);
            if (!__chan_check(chan)) {
                bk_printf("no such channel: %d\r\n", chan);
                __pwm_usage();
                return;
            }
            int id = __get_free_pwm(chan);
            BK_LOG_ON_ERR(bk_pwm_stop(__pwm_channel_remap(pconf[id].ch)));
            pconf[id].is_running = 0;
            __chan_dump(&pconf[id], "stop");
        }
    } else if (os_strcmp(argv[1], "free") == 0) {
        bk_printf("stop all & free\r\n");
        // stop
        for (int i = 0; i < 5; i++) {
            if (pconf[i].ch != 0) {
                BK_LOG_ON_ERR(bk_pwm_stop(__pwm_channel_remap(pconf[i].ch)));
                BK_LOG_ON_ERR(bk_pwm_deinit(__pwm_channel_remap(pconf[i].ch)));
                pconf[i].is_running = 0;
            }
        }
        // free
        if (pconf) {
            tkl_system_free(pconf);
            pconf = NULL;
        }
    } else if (os_strcmp(argv[1], "show") == 0) {
        bk_printf("pwm set info:\r\n");
        for (int i = 0; i < 5; i++) {
            if (pconf[i].ch != 0) {
                bk_printf("channel: %2d,freq: %d,\tduty: %d%%,\tperiod: %d,\thtime: %d,\tstatus: %s\r\n",
                        pconf[i].ch, pconf[i].freq, pconf[i].duty, pconf[i].conf.period_cycle,
                        pconf[i].conf.duty_cycle,
                        (pconf[i].is_running == 1? "running": "stop"));
            }
        }
    } else if (os_strcmp(argv[1], "help") == 0) {
        __pwm_usage();
        return;
    } else {
        bk_printf("error, unknow command\r\n");
        return;
    }
}

#include "bk_saradc.h"
#include <driver/adc.h>
#include "adc_statis.h"
#include <os/os.h>
#include "sys_driver.h"

static void cli_adc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int32_t buf[10];
    uint16_t value   = 0;
    float cali_value = 0;
    adc_chan_t adc_chan = ADC_6;

    static uint8_t is_init = 0;

    memset(buf, 0, sizeof(buf));

    if (!is_init) {
        BK_LOG_ON_ERR(bk_adc_driver_init());
        is_init = 1;
    }

    BK_LOG_ON_ERR(bk_adc_acquire());
    sys_drv_set_ana_pwd_gadc_buf(1);
    BK_LOG_ON_ERR(bk_adc_init(adc_chan));
    adc_config_t config = {0};

    config.chan = adc_chan;
    config.adc_mode = 3;
    config.src_clk = 1;
    config.clk = 0x30e035;
    config.saturate_mode = 4;
    config.steady_ctrl= 7;
    config.adc_filter = 0;
    if(config.adc_mode == ADC_CONTINUOUS_MODE) {
        config.sample_rate = 0;
    }

    BK_LOG_ON_ERR(bk_adc_set_config(&config));
    BK_LOG_ON_ERR(bk_adc_enable_bypass_clalibration());
    BK_LOG_ON_ERR(bk_adc_start());
    BK_LOG_ON_ERR(bk_adc_read(&value, 1000));

    if(adc_chan == 0)
    {
        //cali_value = ((float)value/4096*5)*1.2*1000;
        cali_value = saradc_calculate(value);
        cali_value = cali_value*5/2;
    }
    else if(adc_chan == 7 || adc_chan == 8 || adc_chan == 9)
    {
        bk_printf("adc_chan %d has been used\r\n", adc_chan);
    }
    else
    {
        //cali_value = ((float)value/4096*2)*1.2*1000;
        cali_value = saradc_calculate(value);
    }

    bk_adc_stop();
    sys_drv_set_ana_pwd_gadc_buf(0);
    bk_adc_deinit(adc_chan);
    bk_adc_release();
    bk_printf("adc read: %d, %f\r\n", value, cali_value);

//    BK_LOG_ON_ERR(bk_adc_driver_deinit());
}

#define TUYA_TEST_CMD_CNT (sizeof(s_sinfo_commands) / sizeof(struct cli_command))
static const struct cli_command s_sinfo_commands[] = {
    {"info", "system info", cli_task_cpuload_cmd },
    {"lp", "set wifi dtim", cli_wifi_set_interval_cmd},
    {"rf_cali", "set rf calibration flag, just for test", cli_rf_set_cali_cmd},
    {"xpwm", "pwm test", cli_pwm_cmd},
    {"xadc", "adc test", cli_adc_cmd},
};

int cli_tuya_test_init(void)
{
    return cli_register_commands(s_sinfo_commands, TUYA_TEST_CMD_CNT);
}


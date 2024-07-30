#include "tkl_sleep.h"
#include <modules/pm.h>
#include "tkl_wakeup.h"
#include "tkl_wifi.h"
#include <driver/gpio.h>
#if CONFIG_AON_RTC
#include <driver/aon_rtc_types.h>
#include <driver/hal/hal_aon_rtc_types.h>
#include <driver/aon_rtc.h>
#endif

#define TUYA_CPU_SUPER_DEEP_SLEEP 2

#if CONFIG_AON_RTC
extern OPERATE_RET tkl_wakeup_source_get(TUYA_WAKEUP_SOURCE_BASE_CFG_T *param, uint32_t *status);
static void __pm_rtc_callback(aon_rtc_id_t id, uint8_t *name_p, void *param)
{
    bk_printf("__pm_rtc_callback[%d]\r\n",bk_pm_exit_low_vol_wakeup_source_get());
}

static void __pm_gpio_callback(gpio_id_t gpio_id)
{
    bk_printf("__pm_gpio_callback[%d]\r\n",bk_pm_exit_low_vol_wakeup_source_get());
}
#if defined(ENABLE_WIFI_ULTRA_LOWPOWER) && (ENABLE_WIFI_ULTRA_LOWPOWER == 1)
#define RTC_TIME 1000
alarm_info_t low_valtage_alarm;
void _bk_rtc_wakeup_register(unsigned int rtc_time) 
{
    bk_printf("%s\r\n", __func__);
    memcpy(low_valtage_alarm.name, "rtc_wakeup", sizeof("rtc_wakeup"));
    low_valtage_alarm.period_tick = rtc_time*AON_RTC_MS_TICK_CNT;
    low_valtage_alarm.period_cnt = 0xFFFFFFFF;
    low_valtage_alarm.callback = NULL;
    low_valtage_alarm.param_p = NULL;

    //force unregister previous if doesn't finish.
    bk_alarm_unregister(AON_RTC_ID_1, low_valtage_alarm.name);
    bk_alarm_register(AON_RTC_ID_1, &low_valtage_alarm);
    bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_RTC, NULL);
}

void _bk_rtc_wakeup_unregister(void)
{
    bk_printf("%s\r\n", __func__);
    bk_alarm_unregister(AON_RTC_ID_1, low_valtage_alarm.name);
}
#endif
#endif

/**
* @brief Set the low power mode of CPU
*
* @param[in] enable: enable switch
* @param[in] mode:   cpu sleep mode
*
* @note This API is used for setting the low power mode of CPU.
*
* @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
*/
OPERATE_RET tkl_cpu_sleep_mode_set(BOOL_T enable, TUYA_CPU_SLEEP_MODE_E mode)
{
    bk_printf("-- cpu sleep set enable:%d, mode:%d\r\n", enable, mode);

    if(mode == TUYA_CPU_SLEEP) {
#if defined(ENABLE_WIFI_ULTRA_LOWPOWER) && (ENABLE_WIFI_ULTRA_LOWPOWER == 1)
        if(enable) {
            bk_pm_module_vote_sleep_ctrl(12, 1, 0);
            //os_printf("bk_pm_module_vote_sleep_ctrl enable !!!\r\n");
        }else {
            bk_pm_module_vote_sleep_ctrl(12, 0, 0);
            // bk7236 连上路由后，cpu 一直保持在睡眠状态，唤醒周期由wifi唤醒决定
            //os_printf("bk_pm_module_vote_sleep_ctrl disable !!!\r\n");
        }
#else 
        //默认cpu就是睡眠模式（调度和中断能自己唤醒），不需要设置
#endif
    } else if (mode == TUYA_CPU_DEEP_SLEEP) {
        if(enable) {
            // PM_MODE_DEEP_SLEEP
            bk_printf("prepare to deepsleep\r\n");

            // set wakeup source
            uint32_t status = 0;
            TUYA_WAKEUP_SOURCE_BASE_CFG_T  wakeup[2];
            OPERATE_RET ret = tkl_wakeup_source_get(wakeup, &status);
            if (ret != OPRT_OK) {
                bk_printf("get wakeup source failed\r\n");
                return OPRT_OS_ADAPTER_CPU_LPMODE_SET_FAILED;
            }

            bk_printf("wakeup status: %x\r\n", status);
            if (status & 0x01) {
                // GPIO
                #if CONFIG_GPIO_WAKEUP_SUPPORT
                gpio_id_t id = wakeup[0].wakeup_para.gpio_param.gpio_num;
                TUYA_GPIO_LEVEL_E level = wakeup[0].wakeup_para.gpio_param.level;
                gpio_int_type_t int_type = GPIO_INT_TYPE_LOW_LEVEL;

                if (level == TUYA_GPIO_LEVEL_LOW) {
                    int_type = GPIO_INT_TYPE_LOW_LEVEL;
                } else {
                    int_type = GPIO_INT_TYPE_HIGH_LEVEL;
                }

                bk_printf("gpio wakeup: %d\r\n", id);
                bk_gpio_register_isr(id, __pm_gpio_callback);
                bk_gpio_register_wakeup_source(id, int_type);
                bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_GPIO, NULL);
                #endif //CONFIG_GPIO_WAKEUP_SUPPORT
            }

            if (status & 0x02) {
                // RTC
                #if CONFIG_AON_RTC
                uint32_t ms = wakeup[1].wakeup_para.timer_param.ms;
                uint32_t rtc_ms_tick = bk_rtc_get_clock_freq()/1000;
                aon_pmu_drv_lpo_src_set(PM_LPO_SRC_ROSC);
                alarm_info_t low_valtage_alarm = {
                    "low_vol", ms * rtc_ms_tick, 1, __pm_rtc_callback, NULL
                };
                if(ms < 500) {
                    os_printf("param %d invalid ! must > %dms.\r\n", ms, 500);
                    return OPRT_OS_ADAPTER_CPU_LPMODE_SET_FAILED;
                }
                //force unregister previous if doesn't finish.
                bk_alarm_unregister(AON_RTC_ID_1, low_valtage_alarm.name);
                bk_alarm_register(AON_RTC_ID_1, &low_valtage_alarm);
                bk_printf("rtc wakeup: %d\r\n", ms);
                #endif //CONFIG_AON_RTC
                bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_RTC, NULL);
            }

            // 3. stop cpu 1
            #if CONFIG_SYS_CPU0 && (CONFIG_CPU_CNT > 1)
            stop_cpu1_core();
            #endif

            // 4. set deepsleep mode
            bk_pm_sleep_mode_set(PM_MODE_DEEP_SLEEP);
        }
    } else if(mode == TUYA_CPU_SUPER_DEEP_SLEEP) {
        if(enable) {
            os_printf("prepare to super deepsleep\r\n");
            // 1. disable tuya ble
            //tuya_ble_set_serv_switch(false);
            // 2. set wakeup source: usb plug
            usbplug_wakeup_param_t  usbplug_wakeup_param = {0};
            bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_USBPLUG, &usbplug_wakeup_param);
            // 3. stop cpu 1
            #if CONFIG_SYS_CPU0 && (CONFIG_CPU_CNT > 1)
            stop_cpu1_core();
            #endif
            // 4. set deepsleep mode
            bk_pm_sleep_mode_set(PM_MODE_SUPER_DEEP_SLEEP);
        }
    } else {
        return OPRT_OS_ADAPTER_CPU_LPMODE_SET_FAILED;
    }

    return OPRT_OK;
}

#if defined(ENABLE_WIFI_ULTRA_LOWPOWER) && (ENABLE_WIFI_ULTRA_LOWPOWER == 1)
#define AP_CONNECT_POWER_RATIO      (220)
#define NET_CONNECT_POWER_RATIO     (80)

OPERATE_RET tkl_get_cpu_sleep_param(uint32_t* ap_conn_power_ratio, uint32_t* net_conn_power_ratio)
{
    *ap_conn_power_ratio = AP_CONNECT_POWER_RATIO;
    *net_conn_power_ratio = NET_CONNECT_POWER_RATIO;
    return OPRT_OK;
}
#endif

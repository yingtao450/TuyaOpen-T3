#include "tkl_sleep.h"
#include <modules/pm.h>
#include "tkl_wakeup.h"
#include "tkl_wifi.h"
#include <driver/gpio.h>
#include "aon_pmu_driver.h"

#if CONFIG_AON_RTC
#include <driver/aon_rtc_types.h>
#include <driver/hal/hal_aon_rtc_types.h>
#include <driver/aon_rtc.h>
#endif

#define TUYA_CPU_SUPER_DEEP_SLEEP 2

extern void tkl_set_ll_wakeup_source(void);
extern void bk_printf(const char *fmt, ...);
#if CONFIG_AON_RTC
extern OPERATE_RET tkl_wakeup_source_get(TUYA_WAKEUP_SOURCE_BASE_CFG_T *param, uint32_t *status);
extern BOOL_T tkl_get_lp_flag(VOID);
static void __pm_rtc_callback(aon_rtc_id_t id, uint8_t *name_p, void *param)
{
    bk_printf("__pm_rtc_callback[%d]\r\n",bk_pm_exit_low_vol_wakeup_source_get());
}

static void __pm_gpio_callback(gpio_id_t gpio_id)
{
    bk_printf("__pm_gpio_callback[%d]\r\n",bk_pm_exit_low_vol_wakeup_source_get());
}

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
        if(tkl_get_lp_flag()) {
            if(enable) {
                bk_pm_module_vote_sleep_ctrl(12, 1, 0);
                //bk_printf("bk_pm_module_vote_sleep_ctrl enable !!!\r\n");
            }else {
                bk_pm_module_vote_sleep_ctrl(12, 0, 0);
                // bk7236 连上路由后，cpu 一直保持在睡眠状态，唤醒周期由wifi唤醒决定
                //bk_printf("bk_pm_module_vote_sleep_ctrl disable !!!\r\n");
            }
        } else {
            //默认cpu就是睡眠模式（调度和中断能自己唤醒），不需要设置
        }
    } else if (mode == TUYA_CPU_DEEP_SLEEP) {
        if(enable) {
            // PM_MODE_DEEP_SLEEP
            bk_printf("prepare to deepsleep\r\n");

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

            // 4. set deepsleep mode
            bk_pm_sleep_mode_set(PM_MODE_SUPER_DEEP_SLEEP);
        }
    } else {
        return OPRT_OS_ADAPTER_CPU_LPMODE_SET_FAILED;
    }

    return OPRT_OK;
}

#define AP_CONNECT_POWER_RATIO      (220)
#define NET_CONNECT_POWER_RATIO     (80)

OPERATE_RET tkl_get_cpu_sleep_param(uint32_t* ap_conn_power_ratio, uint32_t* net_conn_power_ratio)
{
    *ap_conn_power_ratio = AP_CONNECT_POWER_RATIO;
    *net_conn_power_ratio = NET_CONNECT_POWER_RATIO;
    return OPRT_OK;
}


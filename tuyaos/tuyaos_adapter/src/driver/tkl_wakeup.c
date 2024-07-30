/*
 * tkl_wakeup.c
 * Copyright (C) 2024 cc <cc@tuya>
 *
 * Distributed under terms of the MIT license.
 */

#include "tkl_wakeup.h"

#define WAKEUP_SOURCE_CNT   2
static TUYA_WAKEUP_SOURCE_BASE_CFG_T  *wakeup_source = NULL;
static uint32_t wakeup_source_init_status = 0;

/**
 * @brief wake up source set
 *
 * @param[in] param: wake up source set,
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tkl_wakeup_source_set(const TUYA_WAKEUP_SOURCE_BASE_CFG_T  *param)
{
    if (param == NULL)
        return OPRT_INVALID_PARM;

    if (wakeup_source == NULL) {
        wakeup_source = tkl_system_malloc(WAKEUP_SOURCE_CNT * sizeof(TUYA_WAKEUP_SOURCE_BASE_CFG_T));
        if (wakeup_source == NULL) {
            bk_printf("wakeup source malloc failed\r\n");
            return OPRT_INVALID_PARM;
        }
    }

    if (param->source == TUYA_WAKEUP_SOURCE_GPIO) {
        memcpy(&wakeup_source[0], param, sizeof(TUYA_WAKEUP_SOURCE_BASE_CFG_T));
        bk_printf("set gpio wakeup: %d, %d\r\n",
                wakeup_source[0].wakeup_para.gpio_param.gpio_num,
                wakeup_source[0].wakeup_para.gpio_param.level);
    } else if (param->source == TUYA_WAKEUP_SOURCE_TIMER) {
        memcpy(&wakeup_source[1], param, sizeof(TUYA_WAKEUP_SOURCE_BASE_CFG_T));
        bk_printf("set rtc wakeup: %d\r\n",
                wakeup_source[1].wakeup_para.timer_param.ms);
    } else {
        bk_printf("unsupport source: %d\r\n", param->source);
        return OPRT_INVALID_PARM;
    }
    wakeup_source_init_status |= (1 << param->source);
    bk_printf("wakeup source: %d %x\r\n", param->source, wakeup_source_init_status);

    return OPRT_OK;
}

/**
 * @brief wake up source get
 *
 * @param[in] param:  wake up source get
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tkl_wakeup_source_get(TUYA_WAKEUP_SOURCE_BASE_CFG_T *param, uint32_t *status)
{
    if (param == NULL) {
        return OPRT_INVALID_PARM;
    }

    if (wakeup_source == NULL) {
        bk_printf("wakeup source not set\r\n");
        return OPRT_NOT_FOUND;
    }

    memcpy(param, wakeup_source, WAKEUP_SOURCE_CNT * sizeof(TUYA_WAKEUP_SOURCE_BASE_CFG_T));
    *status = wakeup_source_init_status;

    return OPRT_OK;
}

/**
 * @brief wake up source clear
 *
 * @param[in] param:  wake up source clear,
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tkl_wakeup_source_clear(const TUYA_WAKEUP_SOURCE_BASE_CFG_T *param)
{
    return OPRT_OK;
}




/**
* @file     tkl_hci.h
* @brief    Host-Controller Interface, Sync with Tuya Host Stack
* @version
* @date     2021-10-11
*
* @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
*
*/

#ifndef __TKL_IPC_H__
#define __TKL_IPC_H__

#include "tuya_cloud_types.h"
#include "tuya_error_code.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TKL_CPU_ID_0 = 0,
    TKL_CPU_ID_1,
    TKL_CPU_ID_2,
    TKL_CPU_ID_3,
    TKL_CPU_ID_4,
    TKL_CPU_ID_5,
    TKL_CPU_ID_6,
    TKL_CPU_ID_7,
    TKL_CPU_ID_MAX,
} TKL_IPC_CORE_INDEX_T;

typedef void* TKL_IPC_HANDLE;

typedef OPERATE_RET (*TKL_IPC_FUNC_CB)(UINT8_T *buf, UINT32_T buf_len, void *param);

typedef struct {
    TKL_IPC_FUNC_CB  cb;
} TKL_IPC_CONF_T;

/**
 * @brief   Function for initializing the inter-processor communication
 * @param[in]  id       local cpu id
 * @param[out] handle   ipc handle
 * @return  SUCCESS             Initialized successfully.
 *          ERROR
 * */
OPERATE_RET tkl_ipc_init(TKL_IPC_CONF_T *config, TKL_IPC_HANDLE *handle);

/**
 * @brief   Function for send message between processors
 * @param[out] handle   ipc handle
 * @param   buffer:     message buffer
 * @param   length:     message length
 * @return  SUCCESS             Initialized successfully.
 *          ERROR
 * */
OPERATE_RET tkl_ipc_message_send(TKL_IPC_HANDLE *handle, const UINT8_T *buffer, UINT32_T length);


#ifdef __cplusplus
}
#endif

#endif /* !__TKL_IPC_H__ */

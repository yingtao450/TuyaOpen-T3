/**
 * @file example_https_client.c
 * @brief Demonstrates HTTPS client usage in Tuya SDK applications.
 *
 * This file provides an example of how to use the HTTPS client interface provided by the Tuya SDK to send HTTPS requests
 * and handle responses. It includes initializing the SDK, setting up network connections (both WiFi and wired,
 * depending on the configuration), sending a GET request to a specified URL, and handling the response. The example
 * also demonstrates how to handle network link status changes and perform cleanups.
 *
 * Key operations demonstrated in this file:
 * - Initialization of the Tuya SDK and network manager.
 * - Sending an HTTPS GET request and receiving a response.
 * - Handling network link status changes.
 * - Cleanup and resource management.
 *
 * This example is intended for developers looking to integrate HTTPS communication into their Tuya SDK-based IoT
 * applications, providing a foundation for building applications that interact with web services.
 *
 * @copyright Copyright (c) 2021-2024 Tuya Inc. All Rights Reserved.
 *
 */

#include "tuya_cloud_types.h"
#include "http_client_interface.h"

#include "tal_api.h"
#include "tkl_output.h"
#include "netmgr.h"
#if defined(ENABLE_WIFI) && (ENABLE_WIFI == 1)
#include "netconn_wifi.h"
#endif
#if defined(ENABLE_WIRED) && (ENABLE_WIRED == 1)
#include "netconn_wired.h"
#endif

/***********************************************************
*********************** macro define ***********************
***********************************************************/
#define URL  "httpbin.org"
#define PATH "/get"

#ifdef ENABLE_WIFI
#define DEFAULT_WIFI_SSID "your-ssid-****"
#define DEFAULT_WIFI_PSWD "your-pswd-****"
#endif
/***********************************************************
********************** typedef define **********************
***********************************************************/
#define HTTP_REQUEST_TIMEOUT 10 * 1000

/***********************************************************
********************** variable define *********************
***********************************************************/

/***********************************************************
********************** function define *********************
***********************************************************/

/**
 * @brief  __link_status_cb
 *
 * @param[in] param:Task parameters
 * @return none
 */
OPERATE_RET __link_status_cb(void *data)
{
    int rt = OPRT_OK;
    uint16_t cacert_len = 0;
    uint8_t *cacert = NULL;
    static netmgr_status_e status = NETMGR_LINK_DOWN;
    if (status == (netmgr_status_e)data && NETMGR_LINK_UP == (netmgr_status_e)data)
        return OPRT_OK;

    /* HTTP Response */
    http_client_response_t http_response = {0};

    /* HTTPS cert */
    TUYA_CALL_ERR_RETURN(tuya_iotdns_query_domain_certs(URL, &cacert, &cacert_len));

    /* HTTP headers */
    http_client_header_t headers[] = {{.key = "Content-Type", .value = "application/json"}};

    /* HTTP Request send */
    PR_DEBUG("http request send!");
    http_client_status_t http_status = http_client_request(
        &(const http_client_request_t){.cacert = cacert,
                                       .cacert_len = cacert_len,
                                       .host = URL,
                                       .port = 443,
                                       .method = "GET",
                                       .path = PATH,
                                       .headers = headers,
                                       .headers_count = sizeof(headers) / sizeof(http_client_header_t),
                                       .body = "",
                                       .body_length = 0,
                                       .timeout_ms = HTTP_REQUEST_TIMEOUT},
        &http_response);

    if (HTTP_CLIENT_SUCCESS != http_status) {
        PR_ERR("http_request_send error:%d", http_status);
        rt = OPRT_LINK_CORE_HTTP_CLIENT_SEND_ERROR;
        goto err_exit;
    }

    PR_DEBUG_RAW("http_get_example body: \n%s\n", (char *)http_response.body);
err_exit:
    http_client_free(&http_response);

    return rt;
}

/**
 * @brief user_main
 *
 * @return void
 */
void user_main()
{
    OPERATE_RET rt = OPRT_OK;

    /* basic init */
    tal_log_init(TAL_LOG_LEVEL_DEBUG, 1024, (TAL_LOG_OUTPUT_CB)tkl_log_output);
    tal_kv_init(&(tal_kv_cfg_t){
        .seed = "vmlkasdh93dlvlcy",
        .key = "dflfuap134ddlduq",
    });
    tal_sw_timer_init();
    tal_workq_init();
    tuya_tls_init();
    tuya_register_center_init();
    tal_event_subscribe(EVENT_LINK_STATUS_CHG, "https_client", __link_status_cb, SUBSCRIBE_TYPE_NORMAL);

    // 初始化LWIP
#if defined(ENABLE_LIBLWIP) && (ENABLE_LIBLWIP == 1)
    TUYA_LwIP_Init();
#endif

    // network init
    netmgr_type_e type = 0;
#if defined(ENABLE_WIFI) && (ENABLE_WIFI == 1)
    type |= NETCONN_WIFI;
#endif
#if defined(ENABLE_WIRED) && (ENABLE_WIRED == 1)
    type |= NETCONN_WIRED;
#endif
    netmgr_init(type);

#if defined(ENABLE_WIFI) && (ENABLE_WIFI == 1)
    // connect wifi
    netconn_wifi_info_t wifi_info = {0};
    strcpy(wifi_info.ssid, DEFAULT_WIFI_SSID);
    strcpy(wifi_info.pswd, DEFAULT_WIFI_PSWD);
    netmgr_conn_set(NETCONN_WIFI, NETCONN_CMD_SSID_PSWD, &wifi_info);
#endif

    return;
}

/**
 * @brief main
 *
 * @param argc
 * @param argv
 * @return void
 */
#if OPERATING_SYSTEM == SYSTEM_LINUX
void main(int argc, char *argv[])
{
    user_main();
    while (1) {
        tal_system_sleep(500);
    }
}
#else

/* Tuya thread handle */
static THREAD_HANDLE ty_app_thread = NULL;

/**
 * @brief  task thread
 *
 * @param[in] arg:Parameters when creating a task
 * @return none
 */
static void tuya_app_thread(void *arg)
{
    user_main();

    tal_thread_delete(ty_app_thread);
    ty_app_thread = NULL;
}

void tuya_app_main(void)
{
    THREAD_CFG_T thrd_param = {4096, 4, "tuya_app_main"};
    tal_thread_create_and_start(&ty_app_thread, NULL, NULL, tuya_app_thread, NULL, &thrd_param);
}
#endif
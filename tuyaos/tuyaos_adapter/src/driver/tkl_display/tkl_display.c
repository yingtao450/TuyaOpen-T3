/*
 * tkl_display.c
 * Copyright (C) 2023 cc <cc@tuya>
 *
 * Distributed under terms of the MIT license.
 */
#include "tkl_display_private.h"

#include "tkl_system.h"
#include "tkl_thread.h"
#include "tkl_queue.h"
#include "tkl_memory.h"
#include "tkl_semaphore.h"
#include "tkl_gpio.h"

/***********************************************************
***********************typedef define***********************
***********************************************************/
typedef struct disp_dev_info{
    struct disp_dev_info     *nxt;
    bool                      is_init;
    int                       id;
    TKL_DISP_EVENT_HANDLER_S  event_cbs;
    TKL_DISP_INFO_S           dev_info;
    TKL_DISP_DRV_HANDLE       drv_handle;
    TKL_DISP_DRV_INTFS_T      drv_intfs;
    TKL_DISP_RECT_S           blit_rect;
    TKL_DISP_FRAMEBUFFER_S   *fb_show_buff;
}TKL_DISP_DEV_INFO_T;

typedef struct {
    TKL_DISP_DEV_INFO_T     *display_device;
    TKL_DISP_FRAMEBUFFER_S  *display_fb;
}TKL_DISP_MSG_T;

/***********************************************************
***********************variable define**********************
***********************************************************/
static TKL_SEM_HANDLE        sg_disp_sem_hdl = NULL;
static TKL_DISP_DEV_INFO_T  *sg_disp_dev_info_arr[TKL_DISP_NUM];

/***********************************************************
***********************function define**********************
***********************************************************/
static void __tkl_display_update_blit_rect(TKL_DISP_DEV_INFO_T *p_dev, TKL_DISP_RECT_S *p_rect)
{
    uint32_t src_end_x = 0,  src_end_y = 0, dst_end_x = 0, dst_end_y = 0;

    if(NULL == p_rect) {
        return;
    }

    if(0 == p_dev->blit_rect.height || 0 == p_dev->blit_rect.width) {
        memcpy(&p_dev->blit_rect, p_rect, sizeof(TKL_DISP_RECT_S));
    }else {
        src_end_x = p_rect->x + p_rect->width;
        src_end_y = p_rect->y + p_rect->height;
        dst_end_x = p_dev->blit_rect.x + p_dev->blit_rect.width;
        dst_end_y = p_dev->blit_rect.y + p_dev->blit_rect.height;

        if(src_end_x > dst_end_x) {
            p_dev->blit_rect.width = src_end_x - p_dev->blit_rect.x;
        }

        if(src_end_y > dst_end_y) {
            p_dev->blit_rect.height = src_end_y - p_dev->blit_rect.y;
        }

        if(p_rect->x < p_dev->blit_rect.x) {
            p_dev->blit_rect.x = p_rect->x;
        }

        if(p_rect->y < p_dev->blit_rect.y) {
            p_dev->blit_rect.y = p_rect->y;
        }
    }
}


//获取缓冲区
static TKL_DISP_FRAMEBUFFER_S *__tkl_display_get_show_buff(TKL_DISP_DEV_INFO_T *p_dev)
{
    if(NULL == p_dev) {
        return NULL;
    }

    if(NULL == p_dev->fb_show_buff){
        p_dev->fb_show_buff = tkl_disp_create_framebuffer(p_dev->dev_info.width, p_dev->dev_info.height, p_dev->dev_info.format);
        if(NULL == p_dev->fb_show_buff) {
            return NULL;
        }
    } 

    return p_dev->fb_show_buff;
}

static void __tkl_display_request_show_buff_display(TKL_DISP_DEV_INFO_T *p_dev)
{
    OPERATE_RET ret = OPRT_OK;

    if(NULL == p_dev || NULL == p_dev->fb_show_buff || \
       NULL == p_dev->drv_intfs.TKL_DISP_DRV_DISPLAY_FRAME) {
        return;
    }

    ret = p_dev->drv_intfs.TKL_DISP_DRV_DISPLAY_FRAME(p_dev->drv_handle, p_dev->fb_show_buff, &p_dev->blit_rect);
    if(ret !=  OPRT_OK) {
        bk_printf("drv display frame:%d", ret);
        return;
    }

    tkl_semaphore_wait(sg_disp_sem_hdl, TKL_SEM_WAIT_FOREVER);

    memset(&p_dev->blit_rect, 0x00, sizeof(TKL_DISP_RECT_S));

    return;
}

static TKL_DISP_DEV_INFO_T *__tkl_disp_find_device_info(TKL_DISP_DEVICE_S *device)
{
    TKL_DISP_DEV_INFO_T *tmp_info = NULL, *target_info = NULL;

    if(NULL == device || device->device_port >= TKL_DISP_NUM) {
        return NULL;
    }

    tmp_info = sg_disp_dev_info_arr[device->device_port];
    while(tmp_info) {
        if(tmp_info->id == device->device_id) {
            target_info = tmp_info;
            break;
        }
        tmp_info = tmp_info->nxt;
    }

    return target_info;
}

void tkl_disp_driver_display_frame_complete(void)
{
    tkl_semaphore_post(sg_disp_sem_hdl);
}

/**
 * @brief register display driver
 *
 * @param disp_port display device id
 * @param disp_port display driver port
 * @param intfs     display driver interface
 * @param dev_info  display device infomation
 * @param handle    display driver handle
 * @return OPERATE_RET 0 on success. A negative error code on error.
 */
OPERATE_RET tkl_disp_driver_register(int dev_id, TKL_DISP_PORT_E disp_port, \
                                    TKL_DISP_DRV_INTFS_T *intfs, TKL_DISP_INFO_S *dev_info, \
                                    TKL_DISP_DRV_HANDLE handle)
{
    if(disp_port >= TKL_DISP_NUM || NULL == intfs) {
        return OPRT_INVALID_PARM;
    }

    TKL_DISP_DEV_INFO_T *p_dev = tkl_system_malloc(sizeof(TKL_DISP_DEV_INFO_T));
    if(NULL == p_dev) {
        return OPRT_MALLOC_FAILED;
    }
    memset(p_dev, 0x00, sizeof(TKL_DISP_DEV_INFO_T));

    p_dev->drv_handle = handle;
    memcpy(&p_dev->drv_intfs, intfs, sizeof(TKL_DISP_DRV_INTFS_T));
    memcpy(&p_dev->dev_info, dev_info, sizeof(TKL_DISP_INFO_S));

    if(NULL == sg_disp_dev_info_arr[disp_port]) {
        sg_disp_dev_info_arr[disp_port] = p_dev;
    }else {
        TKL_DISP_DEV_INFO_T *p_tmp_info =sg_disp_dev_info_arr[disp_port];
        while(p_tmp_info->nxt){
            p_tmp_info = p_tmp_info->nxt;
        }
        p_tmp_info->nxt = p_dev;
    }

    return OPRT_OK;
}

/**
 * @brief Init and config display device
 *
 * @param display_device display device
 * @param cfg display configuration
 * @return OPERATE_RET 0 on success. A negative error code on error.
 */
OPERATE_RET tkl_disp_init(TKL_DISP_DEVICE_S *display_device, TKL_DISP_EVENT_HANDLER_S *event_handler)
{
    TKL_DISP_DEV_INFO_T *p_dev = NULL;
    OPERATE_RET ret = OPRT_OK;

    p_dev = __tkl_disp_find_device_info(display_device);
    if(NULL == p_dev) {
        return OPRT_NOT_FOUND;
    }

    if(p_dev->drv_intfs.TKL_DISP_DRV_INIT) {
        ret = p_dev->drv_intfs.TKL_DISP_DRV_INIT(p_dev->drv_handle);
        if(ret !=  OPRT_OK) {
            return ret;
        }
    }

    if(event_handler) {
        p_dev->event_cbs.vsync_cb = event_handler->vsync_cb;
    }

    if(NULL == sg_disp_sem_hdl) {
        ret = tkl_semaphore_create_init(&sg_disp_sem_hdl, 0, 1);
        if(ret != OPRT_OK) {
            return ret;
        }
    }

    tkl_display_framebuffer_init();

    return ret;
}

/**
 * @brief Release display device
 *
 * @param display_device display device
 * @return OPERATE_RET 0 on success. A negative error code on error.
 */
OPERATE_RET tkl_disp_deinit(TKL_DISP_DEVICE_S *display_device)
{
    return OPRT_OK;
}


/**
 * @brief Set display info
 *
 * @param display_device display device
 * @param info display device info
 * @return OPERATE_RET
 */
OPERATE_RET tkl_disp_set_info(TKL_DISP_DEVICE_S *display_device, TKL_DISP_INFO_S *info)
{
    return OPRT_NOT_SUPPORTED;
}


/**
 * @brief Get display info
 *
 * @param display_device display device
 * @param info display device info
 * @return OPERATE_RET 0 on success. A negative error code on error.
 */
OPERATE_RET tkl_disp_get_info(TKL_DISP_DEVICE_S *display_device, TKL_DISP_INFO_S *info)
{
    TKL_DISP_DEV_INFO_T *p_dev = NULL;

    p_dev = __tkl_disp_find_device_info(display_device);
    if(NULL == p_dev) {
        return OPRT_NOT_FOUND;
    }  

    memcpy(info, &p_dev->dev_info, sizeof(TKL_DISP_INFO_S));

    return OPRT_OK;
}


/**
 * @brief
 *
 * @param display_device display device
 * @param buf framebuffer
 * @param rect destination area
 * @return OPERATE_RET 0 on success. A negative error code on error.
 */
OPERATE_RET tkl_disp_blit(TKL_DISP_DEVICE_S *display_device, TKL_DISP_FRAMEBUFFER_S *buf, TKL_DISP_RECT_S *rect)
{
    TKL_DISP_DEV_INFO_T *p_dev = NULL;

    p_dev = __tkl_disp_find_device_info(display_device);
    if(NULL == p_dev) {
        return OPRT_NOT_FOUND;
    }

    TKL_DISP_FRAMEBUFFER_S *p_frame = __tkl_display_get_show_buff(p_dev);

    tkl_disp_framebuffer_overplay(p_frame, buf, rect);

    __tkl_display_update_blit_rect(p_dev, rect);

    return OPRT_OK;
}

/**
 * @brief Fill destination area with color
 *
 * @param display_device display device
 * @param rect destination area to fill
 * @param color color to fill
 * @return OPERATE_RET
 */
OPERATE_RET tkl_disp_fill(TKL_DISP_DEVICE_S *display_device, TKL_DISP_RECT_S *rect, TKL_DISP_COLOR_U color)
{
    TKL_DISP_DEV_INFO_T *p_dev = NULL;
    OPERATE_RET ret = OPRT_OK;
    uint32_t i = 0, color_u16=0;
    uint16_t *pixel_buffer = NULL;

    p_dev = __tkl_disp_find_device_info(display_device);
    if(NULL == p_dev) {
        return OPRT_NOT_FOUND;
    }

    if(NULL == p_dev->drv_intfs.TKL_DISP_DRV_DISPLAY_FRAME) {
        return OPRT_NOT_SUPPORTED;
    }

    TKL_DISP_FRAMEBUFFER_S *frame= __tkl_display_get_show_buff(p_dev);
    if(NULL == frame) {
        return OPRT_INVALID_PARM;
    }

    //fill color
    pixel_buffer = (uint16_t *)frame->buffer;
    color_u16 = color.full & 0xFFFF;
    for(i=0; i<p_dev->dev_info.width*p_dev->dev_info.height; i++) {
        pixel_buffer[i]= color_u16;
    }

    __tkl_display_update_blit_rect(p_dev, rect);

    __tkl_display_request_show_buff_display(p_dev);

    return ret;
}

/**
 * @brief Flush buffers to display device
 *
 * @param display_device display device
 * @return OPERATE_RET 0 on success. A negative error code on error.
 */
OPERATE_RET tkl_disp_flush(TKL_DISP_DEVICE_S *display_device)
{
    TKL_DISP_DEV_INFO_T *p_dev = NULL;

    p_dev = __tkl_disp_find_device_info(display_device);
    if(NULL == p_dev) {
        return OPRT_NOT_FOUND;
    }

    if(NULL == p_dev->drv_intfs.TKL_DISP_DRV_DISPLAY_FRAME) {
        return OPRT_NOT_SUPPORTED;
    }

    __tkl_display_request_show_buff_display(p_dev);

    return OPRT_OK;
}

/**
 * @brief Wait for vsync signal
 *
 * @param display_device display device
 * @return OPERATE_RET 0 on success. A negative error code on error.
 */
OPERATE_RET tkl_disp_wait_vsync(TKL_DISP_DEVICE_S *display_device)
{
    return OPRT_NOT_SUPPORTED;
}


/**
 * @brief Set display brightness(Backlight or HSB)
 *
 * @param display_device display device
 * @param brightness brightness
 * @return OPERATE_RET 0 on success. A negative error code on error.
 */
OPERATE_RET tkl_disp_set_brightness(TKL_DISP_DEVICE_S *display_device, int brightness)
{
    TKL_DISP_DEV_INFO_T *p_dev = NULL;
    OPERATE_RET ret = OPRT_OK;

    p_dev = __tkl_disp_find_device_info(display_device);
    if(NULL == p_dev) {
        return OPRT_NOT_FOUND;
    }

    if(p_dev->drv_intfs.TKL_DISP_SET_BRIGHTNESS) {
        ret = p_dev->drv_intfs.TKL_DISP_SET_BRIGHTNESS(p_dev->drv_handle, brightness);
    }

    return ret;
}


/**
 * @brief Get display brightness(Backlight or HSB)
 *
 * @param display_device display device
 * @param brightness brightness
 * @return OPERATE_RET 0 on success. A negative error code on error.
 */
OPERATE_RET tkl_disp_get_brightness(TKL_DISP_DEVICE_S *display_device, int *brightness)
{
    return OPRT_NOT_SUPPORTED;
}


/**
 * @brief Sets the display screen's power state
 *
 * @param display_device display device
 * @param power_mode power state
 * @return OPERATE_RET 0 on success. A negative error code on error.
 */
OPERATE_RET tkl_disp_set_power_mode(TKL_DISP_DEVICE_S *display_device, TKL_DISP_POWER_MODE_E power_mode)
{
    return OPRT_OK;
}


/**
 * @brief Gets the display screen's power state
 *
 * @param display_device display device
 * @param power_mode power state
 * @return OPERATE_RET 0 on success. A negative error code on error.
 */
OPERATE_RET tkl_disp_get_power_mode(TKL_DISP_DEVICE_S *display_device, TKL_DISP_POWER_MODE_E *power_mode)
{
    return OPRT_OK;
}


/**
 * @brief Alloc mapped framebuffer or layer
 *
 * @param display_device display device
 * @return void* Pointer to mapped memory
 */
TKL_DISP_FRAMEBUFFER_S *tkl_disp_alloc_framebuffer(TKL_DISP_DEVICE_S *display_device)
{
    return NULL;
}


/**
 * @brief Free mapped framebuffer or layer
 *
 * @param display_device display device
 * @param buf Pointer to mapped memory
 * @return void
 */
void tkl_disp_free_framebuffer(TKL_DISP_DEVICE_S *display_device, TKL_DISP_FRAMEBUFFER_S *buf)
{

}

/**
 * @brief Get capabilities supported by display(For external display device. e.g. HDMI/VGA)
 *
 * @param display_device display device
 * @param cfg configurations
 * @return OPERATE_RET 0 on success. A negative error code on error.
 */
OPERATE_RET tkl_disp_get_capabilities(TKL_DISP_DEVICE_S *display_device, TKL_DISP_INFO_S **cfg)
{
    return OPRT_OK;
}


/**
 * @brief Free capabilities get by tkl_disp_get_capabilities()
 *
 * @param display_device display device
 * @param cfg configurations
 * @return OPERATE_RET 0 on success. A negative error code on error.
 */
OPERATE_RET tkl_disp_free_capabilities(TKL_DISP_DEVICE_S *display_device, TKL_DISP_INFO_S *cfg)
{
    return OPRT_OK;
}







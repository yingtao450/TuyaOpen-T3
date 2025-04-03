/**
 * @file tkl_display_private.h
 * @version 0.1
 * @date 2025-03-11
 */

#ifndef __TKL_DISPLAY_PRIVATE_H__
#define __TKL_DISPLAY_PRIVATE_H__

#include <os/os.h>
#include <driver/lcd.h>
#include <driver/media_types.h>

#include "tkl_display.h"
#include "tkl_disp_driver.h"
#include "tkl_disp_drv_lcd.h"


#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************
************************macro define************************
***********************************************************/


/***********************************************************
***********************typedef define***********************
***********************************************************/


/***********************************************************
********************function declaration********************
***********************************************************/
OPERATE_RET tkl_disp_convert_pixel_fmt_tkl_to_bk(TKL_DISP_PIXEL_FMT_E src_fmt,  pixel_format_t *p_dst_fmt);

uint8_t tkl_disp_convert_pixel_fmt_to_size(TKL_DISP_PIXEL_FMT_E format);


void tkl_display_dma2d_init(void);

void tkl_dispaly_dma2d_framebuffer_transfer(TKL_DISP_FRAMEBUFFER_S *dst_fb, TKL_DISP_FRAMEBUFFER_S *src_fb);


void tkl_display_framebuffer_init(void);

TKL_DISP_FRAMEBUFFER_S *tkl_disp_create_framebuffer(int width, int height, TKL_DISP_PIXEL_FMT_E format);

void tkl_disp_release_framebuffer(TKL_DISP_FRAMEBUFFER_S *buf);

bool tkl_disp_is_rect_in_framebuffer_range(TKL_DISP_RECT_S *rect, TKL_DISP_FRAMEBUFFER_S *fb);

void tkl_disp_framebuffer_overplay(TKL_DISP_FRAMEBUFFER_S *dst, TKL_DISP_FRAMEBUFFER_S *src, TKL_DISP_RECT_S *rect);



#ifdef __cplusplus
}
#endif

#endif /* __TKL_DISPLAY_PRIVATE_H__ */

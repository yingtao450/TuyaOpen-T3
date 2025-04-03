/**
 * @file tkl_display_convert.c
 * @version 0.1
 * @date 2025-03-11
 */


#include "tkl_display_private.h"

/***********************************************************
************************macro define************************
***********************************************************/


/***********************************************************
***********************typedef define***********************
***********************************************************/


/***********************************************************
********************function declaration********************
***********************************************************/


/***********************************************************
***********************variable define**********************
***********************************************************/


/***********************************************************
***********************function define**********************
***********************************************************/
OPERATE_RET tkl_disp_convert_pixel_fmt_tkl_to_bk(TKL_DISP_PIXEL_FMT_E src_fmt,  pixel_format_t *p_dst_fmt)
{
    pixel_format_t fmt = 0;

    if(NULL == p_dst_fmt) {
        return OPRT_INVALID_PARM;
    }

    if (src_fmt == TKL_DISP_PIXEL_FMT_RGB888) {
        fmt = PIXEL_FMT_RGB888;
    } else if (src_fmt == TKL_DISP_PIXEL_FMT_RGB666) {
        fmt = PIXEL_FMT_RGB666;
    }else if (src_fmt == TKL_DISP_PIXEL_FMT_RGB565) {
        fmt = PIXEL_FMT_RGB565;
    }else if (src_fmt == TKL_DISP_PIXEL_FMT_RGB565_LE) {
        fmt = PIXEL_FMT_RGB565_LE;
    } else {
        bk_printf("fmt %d not support now\r\n", src_fmt);
        return OPRT_NOT_SUPPORTED;
    }

    *p_dst_fmt = fmt;

    return OPRT_OK;
}

uint8_t tkl_disp_convert_pixel_fmt_to_size(TKL_DISP_PIXEL_FMT_E format)
{
    uint8_t pixel_size = 2;

    switch(format) {
        case TKL_DISP_PIXEL_FMT_ABGR:
        case TKL_DISP_PIXEL_FMT_RGBX:
        case TKL_DISP_PIXEL_FMT_RGBA:
        case TKL_DISP_PIXEL_FMT_ARGB:
            pixel_size = 4;
            break;
        case TKL_DISP_PIXEL_FMT_RGB565:    
        case TKL_DISP_PIXEL_FMT_RGB565_LE: 
        case TKL_DISP_PIXEL_FMT_YUYV:
        case TKL_DISP_PIXEL_FMT_VUYY:
            pixel_size = 2;
            break;
        case TKL_DISP_PIXEL_FMT_RGB888:
        case TKL_DISP_PIXEL_FMT_RGB666: 
            pixel_size = 3;
            break;           
        default:
            bk_printf("not support get format:%d fmt size\r\n", format);
            break;
    }

    return pixel_size;
}
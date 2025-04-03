/**
 * @file tkl_display_framebuffer.c
 * @version 0.1
 * @date 2025-03-11
 */

#include "tkl_display_private.h"

#include "tkl_memory.h"
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
void tkl_display_framebuffer_init(void)
{
    return;
}

TKL_DISP_FRAMEBUFFER_S *tkl_disp_create_framebuffer(int width, int height, TKL_DISP_PIXEL_FMT_E format)
{
    TKL_DISP_FRAMEBUFFER_S *fb = NULL;
    uint8_t pixel_size = 0;

    fb = (TKL_DISP_FRAMEBUFFER_S *)tkl_system_malloc(sizeof(TKL_DISP_FRAMEBUFFER_S));
    if(NULL == fb) {
        return NULL;
    }
    memset(fb, 0x00, sizeof(TKL_DISP_FRAMEBUFFER_S));

    fb->rect.width  = width;
    fb->rect.height = height;
    fb->format      = format;

    pixel_size = tkl_disp_convert_pixel_fmt_to_size(format);

    fb->buffer = tkl_system_malloc(width*height*pixel_size);
    if (fb->buffer == NULL) {
		bk_printf("frame_buffer_fb_malloc failed\n");
        tkl_system_free(fb);
		return NULL;
	}
    memset(fb->buffer, 0x00, width*height*pixel_size);

    return fb;
}

void tkl_disp_release_framebuffer(TKL_DISP_FRAMEBUFFER_S *buf)
{
    if(NULL == buf) {
        return;
    }

    if(buf->buffer) {
        tkl_system_free(buf->buffer);
    }

    tkl_system_free(buf);

    return;
}

bool tkl_disp_is_rect_in_framebuffer_range(TKL_DISP_RECT_S *rect, TKL_DISP_FRAMEBUFFER_S *fb)
{
    if(NULL == rect || NULL == fb) {
        return false;
    }

    if(rect->x < fb->rect.x || rect->y < fb->rect.y) {
        return false;
    }

    if(rect->x+rect->width > fb->rect.x+fb->rect.width ||\
       rect->y+rect->height > fb->rect.y+fb->rect.height) {
        return false;
    }

    return true;
}

void tkl_disp_framebuffer_overplay(TKL_DISP_FRAMEBUFFER_S *dst, TKL_DISP_FRAMEBUFFER_S *src, TKL_DISP_RECT_S *rect)
{
    int line = 0;
    uint32_t line_start_src_pos = 0, line_start_dst_pos;
    uint8_t pixel_size = 0;

    if(NULL == dst|| NULL == src || NULL == rect) {
        bk_printf("invalid param\r\n");
        return;
    }

    if(false == tkl_disp_is_rect_in_framebuffer_range(rect, dst)) {
        bk_printf("out of dst fb range\r\n");
        return;
    }

    if(false == tkl_disp_is_rect_in_framebuffer_range(rect, src)) {
        bk_printf("out of src fb range\r\n");
        return;
    }

    pixel_size = tkl_disp_convert_pixel_fmt_to_size(dst->format);

    for(line = 0; line < rect->height; line++) {
        line_start_src_pos = ((rect->y-src->rect.y) + line)*src->rect.width + (rect->x-src->rect.x);
        line_start_src_pos = line_start_src_pos*pixel_size;
        line_start_dst_pos = ((rect->y-dst->rect.y) + line)*dst->rect.width + (rect->x-dst->rect.x);
        line_start_dst_pos = line_start_dst_pos*pixel_size;
        memcpy(((uint8_t *)dst->buffer + line_start_dst_pos), ((uint8_t *)src->buffer+line_start_src_pos), rect->width*pixel_size);
    }                                                                                                                                                                                       
}
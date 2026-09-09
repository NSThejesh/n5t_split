#pragma once
#include <lvgl.h>
#include <zephyr/kernel.h>
#include "util.h"
#define CANVAS_BUF_SIZE LV_CANVAS_BUF_SIZE(CANVAS_W, CANVAS_H, LV_COLOR_FORMAT_GET_BPP(CANVAS_COLOR_FORMAT), LV_DRAW_BUF_STRIDE_ALIGN)
struct zmk_widget_screen { sys_snode_t node; lv_obj_t *obj; uint8_t cbuf[CANVAS_BUF_SIZE]; struct status_state state; };
int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget);

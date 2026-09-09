#include <stdlib.h>
#include <zephyr/kernel.h>
#include "animation.h"
#define DIA 8
static lv_obj_t *anim_canvas;
static lv_timer_t *anim_timer;
static uint8_t anim_frame;
static void draw_diamond(lv_obj_t *canvas, uint8_t f) {
    fill_background(canvas);
    lv_draw_line_dsc_t d;
    init_line_dsc(&d, LVGL_FOREGROUND, 1);
    int cx = 12, cy = 9;
    int w = 3 + (f % 5), h = 4 + ((f * 2) % 6);
    if (w > 10) w = 10;
    if (h > 8) h = 8;
    lv_point_t top[2] = {{cx, cy - h}, {cx + w, cy}};
    lv_point_t right[2] = {{cx + w, cy}, {cx, cy + h}};
    lv_point_t bot[2] = {{cx, cy + h}, {cx - w, cy}};
    lv_point_t left[2] = {{cx - w, cy}, {cx, cy - h}};
    canvas_draw_line(canvas, top, 2, &d);
    canvas_draw_line(canvas, right, 2, &d);
    canvas_draw_line(canvas, bot, 2, &d);
    canvas_draw_line(canvas, left, 2, &d);
    if (f % 2 == 0) {
        lv_draw_line_dsc_t hline;
        init_line_dsc(&hline, LVGL_FOREGROUND, 1);
        lv_point_t m[2] = {{cx - w, cy}, {cx + w, cy}};
        canvas_draw_line(canvas, m, 2, &hline);
    }
}
static void anim_cb(lv_timer_t *t) {
    (void)t;
    anim_frame = (anim_frame + 1) % DIA;
    draw_diamond(anim_canvas, anim_frame);
}
void draw_animation(lv_obj_t *parent) {
    anim_canvas = lv_canvas_create(parent);
    static uint8_t abuf[LV_CANVAS_BUF_SIZE(24, 18, 1, LV_DRAW_BUF_STRIDE_ALIGN)];
    lv_canvas_set_buffer(anim_canvas, abuf, 24, 18, LV_COLOR_FORMAT_I1);
    lv_obj_align(anim_canvas, LV_ALIGN_BOTTOM_MID, 10, 0);
#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_ANIMATION)
    anim_frame = 0;
    draw_diamond(anim_canvas, 0);
    uint32_t per = CONFIG_NICE_VIEW_GEM_ANIMATION_MS / DIA;
    if (per < 50) per = 50;
    anim_timer = lv_timer_create(anim_cb, per, NULL);
#else
    int n = CONFIG_NICE_VIEW_GEM_ANIMATION_FRAME % DIA;
    if (n < 0) n = 0;
    draw_diamond(anim_canvas, (uint8_t)n);
#endif
}

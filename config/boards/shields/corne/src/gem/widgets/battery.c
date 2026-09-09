#include <zephyr/kernel.h>
#include <stdio.h>
#include "battery.h"
#include "../assets/custom_fonts.h"
LV_IMAGE_DECLARE(bolt);
void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t left;
    init_label_dsc(&left, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    canvas_draw_text(canvas, 64, 0, 24, &left, "BAT");
    lv_draw_label_dsc_t right;
    init_label_dsc(&right, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_RIGHT);
    char text[10] = {};
    snprintf(text, sizeof(text), "%i%%", state->battery);
    if (state->charging) {
        lv_draw_image_dsc_t img;
        lv_draw_image_dsc_init(&img);
        canvas_draw_text(canvas, 88, 0, 26, &right, text);
        canvas_draw_img(canvas, 121, 1, &bolt, &img);
    } else {
        canvas_draw_text(canvas, 88, 0, 36, &right, text);
    }
}

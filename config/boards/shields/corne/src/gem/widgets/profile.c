#include <zephyr/kernel.h>
#include "profile.h"
LV_IMG_DECLARE(profiles);
void draw_profile_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_image_dsc_t d;
    lv_draw_image_dsc_init(&d);
    canvas_draw_img(canvas, 0, 24, &profiles, &d);
    lv_draw_rect_dsc_t r;
    init_rect_dsc(&r, LVGL_FOREGROUND);
    canvas_draw_rect(canvas, 0 + state->active_profile_index * 7, 24, 3, 3, &r);
}

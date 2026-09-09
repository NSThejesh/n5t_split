#include <zephyr/kernel.h>
#include "output.h"
#include "../assets/custom_fonts.h"
LV_IMG_DECLARE(bt_no_signal);
LV_IMG_DECLARE(bt_unbonded);
LV_IMG_DECLARE(bt);
LV_IMG_DECLARE(usb);
static void draw_usb(lv_obj_t *c) { lv_draw_image_dsc_t d; lv_draw_image_dsc_init(&d); canvas_draw_img(c, 30, 1, &usb, &d); }
static void draw_unbonded(lv_obj_t *c) { lv_draw_image_dsc_t d; lv_draw_image_dsc_init(&d); canvas_draw_img(c, 29, 0, &bt_unbonded, &d); }
static void draw_disc(lv_obj_t *c) { lv_draw_image_dsc_t d; lv_draw_image_dsc_init(&d); canvas_draw_img(c, 33, 0, &bt_no_signal, &d); }
static void draw_conn(lv_obj_t *c) { lv_draw_image_dsc_t d; lv_draw_image_dsc_init(&d); canvas_draw_img(c, 33, 0, &bt, &d); }
void draw_output_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t l;
    init_label_dsc(&l, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    canvas_draw_text(canvas, 0, 0, 24, &l, "SIG");
    lv_draw_rect_dsc_t r;
    init_rect_dsc(&r, LVGL_FOREGROUND);
    canvas_draw_rect(canvas, 26, 0, 22, 13, &r);
#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    switch (state->selected_endpoint.transport) {
    case ZMK_TRANSPORT_USB: draw_usb(canvas); break;
    case ZMK_TRANSPORT_BLE:
        if (state->active_profile_bonded) { if (state->active_profile_connected) draw_conn(canvas); else draw_disc(canvas); }
        else draw_unbonded(canvas);
        break;
    }
#else
    if (state->connected) draw_conn(canvas); else draw_disc(canvas);
#endif
}

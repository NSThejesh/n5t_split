#include <zephyr/kernel.h>
#include <stdio.h>
#include <string.h>
#include "layer.h"
#include "../assets/custom_fonts.h"
void draw_layer_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t l;
    init_label_dsc(&l, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_CENTER);
    char text[10] = {};
    if (state->layer_label == NULL) snprintf(text, sizeof(text), "%i", state->layer_index);
    else { strncpy(text, state->layer_label, 9); text[9] = 0; to_uppercase(text); }
    canvas_draw_text(canvas, 52, 22, 76, &l, text);
}

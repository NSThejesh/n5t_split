#include <zephyr/kernel.h>
#include <stdio.h>
#include "wpm.h"
#include "../assets/custom_fonts.h"
void draw_wpm_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t ll;
    init_label_dsc(&ll, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    canvas_draw_text(canvas, 0, 13, 24, &ll, "WPM");
    lv_draw_label_dsc_t lr;
    init_label_dsc(&lr, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_RIGHT);
    char t[6] = {};
    snprintf(t, sizeof(t), "%d", state->wpm[9]);
    canvas_draw_text(canvas, 24, 13, 30, &lr, t);
    lv_draw_line_dsc_t ld;
    init_line_dsc(&ld, LVGL_FOREGROUND, 1);
    lv_point_t pts[10];
    const int baseY = 30, h = 12, x0 = 60;
#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_WPM_FIXED_RANGE)
    int max = CONFIG_NICE_VIEW_GEM_WPM_FIXED_RANGE_MAX;
    if (max == 0) max = 100;
    for (int i = 0; i < 10; i++) { int v = state->wpm[i]; if (v > max) v = max; if (v < 0) v = 0; pts[i].x = x0 + (int)(i * 6.8); pts[i].y = baseY - (v * h / max); }
#else
    int mx = 0, mn = 256;
    for (int i = 0; i < 10; i++) { if ((int)state->wpm[i] > mx) mx = state->wpm[i]; if ((int)state->wpm[i] < mn) mn = state->wpm[i]; }
    int rg = mx - mn; if (rg == 0) rg = 1;
    for (int i = 0; i < 10; i++) { pts[i].x = x0 + (int)(i * 6.8); pts[i].y = baseY - ((state->wpm[i] - mn) * h / rg); }
#endif
    canvas_draw_line(canvas, pts, 10, &ld);
}

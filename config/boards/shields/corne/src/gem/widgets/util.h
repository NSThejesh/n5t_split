#pragma once
#include <lvgl.h>
#include <zmk/endpoints.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define CANVAS_W 128
#define CANVAS_H 32
#define CANVAS_SIZE 128
#define CANVAS_COLOR_FORMAT LV_COLOR_FORMAT_I1
#define LVGL_BACKGROUND lv_color_black()
#define LVGL_FOREGROUND lv_color_white()
struct status_state {
    uint8_t battery;
    bool charging;
    struct zmk_endpoint_instance selected_endpoint;
    int active_profile_index;
    bool active_profile_connected;
    bool active_profile_bonded;
    uint8_t layer_index;
    const char *layer_label;
    uint8_t wpm[10];
    bool connected;
};
void to_uppercase(char *str);
void fill_background(lv_obj_t *canvas);
void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color);
void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color, uint8_t width);
void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color, const lv_font_t *font, lv_text_align_t align);
void canvas_draw_rect(lv_obj_t *canvas, int32_t x, int32_t y, int32_t w, int32_t h, lv_draw_rect_dsc_t *rect_dsc);
void canvas_draw_text(lv_obj_t *canvas, int32_t x, int32_t y, int32_t max_w, lv_draw_label_dsc_t *label_dsc, const char *text);
void canvas_draw_line(lv_obj_t *canvas, const lv_point_t *points, uint32_t point_cnt, lv_draw_line_dsc_t *line_dsc);
void canvas_draw_img(lv_obj_t *canvas, int32_t x, int32_t y, const void *src, lv_draw_image_dsc_t *img_dsc);

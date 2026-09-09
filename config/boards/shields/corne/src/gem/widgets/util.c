#include <zephyr/kernel.h>
#include "util.h"
#include <ctype.h>
#include <string.h>
void to_uppercase(char *str) { for (int i = 0; str[i] != 0; i++) str[i] = toupper((unsigned char)str[i]); }
void fill_background(lv_obj_t *canvas) { lv_canvas_fill_bg(canvas, LVGL_BACKGROUND, LV_OPA_COVER); }
void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color, const lv_font_t *font, lv_text_align_t align) { lv_draw_label_dsc_init(label_dsc); label_dsc->color = color; label_dsc->font = font; label_dsc->align = align; }
void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color) { lv_draw_rect_dsc_init(rect_dsc); rect_dsc->bg_color = bg_color; }
void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color, uint8_t width) { lv_draw_line_dsc_init(line_dsc); line_dsc->color = color; line_dsc->width = width; }
void canvas_draw_rect(lv_obj_t *canvas, int32_t x, int32_t y, int32_t w, int32_t h, lv_draw_rect_dsc_t *rect_dsc) { lv_layer_t layer; lv_canvas_init_layer(canvas, &layer); lv_area_t c = {x, y, x + w - 1, y + h - 1}; lv_draw_rect(&layer, rect_dsc, &c); lv_canvas_finish_layer(canvas, &layer); }
void canvas_draw_text(lv_obj_t *canvas, int32_t x, int32_t y, int32_t max_w, lv_draw_label_dsc_t *label_dsc, const char *text) { lv_layer_t layer; lv_canvas_init_layer(canvas, &layer); label_dsc->text = text; lv_area_t c = {x, y, x + max_w, y + CANVAS_H}; lv_draw_label(&layer, label_dsc, &c); lv_canvas_finish_layer(canvas, &layer); }
void canvas_draw_line(lv_obj_t *canvas, const lv_point_t *points, uint32_t point_cnt, lv_draw_line_dsc_t *line_dsc) { lv_layer_t layer; lv_canvas_init_layer(canvas, &layer); for (uint32_t i = 1; i < point_cnt; i++) { line_dsc->p1.x = points[i-1].x; line_dsc->p1.y = points[i-1].y; line_dsc->p2.x = points[i].x; line_dsc->p2.y = points[i].y; lv_draw_line(&layer, line_dsc); } lv_canvas_finish_layer(canvas, &layer); }
void canvas_draw_img(lv_obj_t *canvas, int32_t x, int32_t y, const void *src, lv_draw_image_dsc_t *img_dsc) { lv_layer_t layer; lv_canvas_init_layer(canvas, &layer); const lv_image_dsc_t *s = src; img_dsc->src = s; lv_area_t c = {x, y, x + (int32_t)s->header.w - 1, y + (int32_t)s->header.h - 1}; lv_draw_image(&layer, img_dsc, &c); lv_canvas_finish_layer(canvas, &layer); }

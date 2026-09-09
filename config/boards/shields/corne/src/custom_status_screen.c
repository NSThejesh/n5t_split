#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);
#include <zmk/display.h>
#include <lvgl.h>
#include "gem/assets/pixel_operator_mono.c"
#include "gem/assets/custom_fonts.h"
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
#include "gem/widgets/screen.h"
static struct zmk_widget_screen screen_widget;
#else
#include "gem/widgets/screen_peripheral.h"
static struct zmk_widget_screen peri_widget;
#endif
lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);
#if IS_ENABLED(CONFIG_CORNE_CUSTOM_DISPLAY)
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    zmk_widget_screen_init(&screen_widget, screen);
    lv_obj_align(zmk_widget_screen_obj(&screen_widget), LV_ALIGN_TOP_LEFT, 0, 0);
#else
    zmk_widget_screen_init(&peri_widget, screen);
    lv_obj_align(zmk_widget_screen_obj(&peri_widget), LV_ALIGN_TOP_LEFT, 0, 0);
#endif
#endif
    return screen;
}

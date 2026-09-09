#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/events/split_peripheral_status_changed.h>
#include <zmk/split/bluetooth/peripheral.h>
#include <zmk/battery.h>
#include <zmk/display.h>
#include <zmk/usb.h>
#include "animation.h"
#include "battery.h"
#include "output.h"
#include "screen_peripheral.h"
static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);
static void redraw(lv_obj_t *obj, const struct status_state *s) {
    lv_obj_t *cv = lv_obj_get_child(obj, 0);
    fill_background(cv);
    draw_output_status(cv, s);
    draw_battery_status(cv, s);
}
static void batt_cb(struct battery_status_state st) {
    struct zmk_widget_screen *w;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, w, node) {
        w->state.charging = st.usb_present;
        w->state.battery = st.level;
        redraw(w->obj, &w->state);
    }
}
static struct battery_status_state batt_get(const zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);
    return (struct battery_status_state){
        .level = ev ? ev->state_of_charge : zmk_battery_state_of_charge(),
        .usb_present = zmk_usb_is_powered(),
    };
}
ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct battery_status_state, batt_cb, batt_get);
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);
static struct peripheral_status_state per_get(const zmk_event_t *eh) {
    (void)eh;
    return (struct peripheral_status_state){.connected = zmk_split_bt_peripheral_is_connected()};
}
static void per_cb(struct peripheral_status_state st) {
    struct zmk_widget_screen *w;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, w, node) {
        w->state.connected = st.connected;
        redraw(w->obj, &w->state);
    }
}
ZMK_DISPLAY_WIDGET_LISTENER(widget_peripheral_status, struct peripheral_status_state, per_cb, per_get);
ZMK_SUBSCRIPTION(widget_peripheral_status, zmk_split_peripheral_status_changed);
int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_style_bg_color(widget->obj, LVGL_BACKGROUND, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(widget->obj, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(widget->obj, 0, LV_PART_MAIN);
    lv_obj_t *cv = lv_canvas_create(widget->obj);
    lv_canvas_set_buffer(cv, widget->cbuf, CANVAS_W, CANVAS_H, CANVAS_COLOR_FORMAT);
    lv_obj_align(cv, LV_ALIGN_TOP_LEFT, 0, 0);
    draw_animation(widget->obj);
    sys_slist_append(&widgets, &widget->node);
    widget_battery_status_init();
    widget_peripheral_status_init();
    return 0;
}
lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }

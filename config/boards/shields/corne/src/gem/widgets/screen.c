#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/events/wpm_state_changed.h>
#include <zmk/battery.h>
#include <zmk/ble.h>
#include <zmk/display.h>
#include <zmk/endpoints.h>
#include <zmk/keymap.h>
#include <zmk/usb.h>
#include <zmk/wpm.h>
#include "battery.h"
#include "layer.h"
#include "output.h"
#include "profile.h"
#include "screen.h"
#include "wpm.h"
static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);
static void redraw(lv_obj_t *obj, const struct status_state *s) {
    lv_obj_t *cv = lv_obj_get_child(obj, 0);
    fill_background(cv);
    draw_output_status(cv, s);
    draw_battery_status(cv, s);
    draw_wpm_status(cv, s);
    draw_profile_status(cv, s);
    draw_layer_status(cv, s);
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
static void layer_cb(struct layer_status_state st) {
    struct zmk_widget_screen *w;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, w, node) {
        w->state.layer_index = st.index;
        w->state.layer_label = st.label;
        redraw(w->obj, &w->state);
    }
}
static struct layer_status_state layer_get(const zmk_event_t *eh) {
    (void)eh;
    uint8_t i = zmk_keymap_highest_layer_active();
    return (struct layer_status_state){.index = i, .label = zmk_keymap_layer_name(i)};
}
ZMK_DISPLAY_WIDGET_LISTENER(widget_layer_status, struct layer_status_state, layer_cb, layer_get);
ZMK_SUBSCRIPTION(widget_layer_status, zmk_layer_state_changed);
static void out_cb(struct output_status_state st) {
    struct zmk_widget_screen *w;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, w, node) {
        w->state.selected_endpoint = st.selected_endpoint;
        w->state.active_profile_index = st.active_profile_index;
        w->state.active_profile_connected = st.active_profile_connected;
        w->state.active_profile_bonded = st.active_profile_bonded;
        redraw(w->obj, &w->state);
    }
}
static struct output_status_state out_get(const zmk_event_t *eh) {
    (void)eh;
    return (struct output_status_state){
        .selected_endpoint = zmk_endpoint_get_selected(),
        .active_profile_index = zmk_ble_active_profile_index(),
        .active_profile_connected = zmk_ble_active_profile_is_connected(),
        .active_profile_bonded = !zmk_ble_active_profile_is_open()
    };
}
ZMK_DISPLAY_WIDGET_LISTENER(widget_output_status, struct output_status_state, out_cb, out_get);
ZMK_SUBSCRIPTION(widget_output_status, zmk_endpoint_changed);
ZMK_SUBSCRIPTION(widget_output_status, zmk_usb_conn_state_changed);
#if defined(CONFIG_ZMK_BLE)
ZMK_SUBSCRIPTION(widget_output_status, zmk_ble_active_profile_changed);
#endif
static void wpm_cb(struct wpm_status_state st) {
    struct zmk_widget_screen *w;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, w, node) {
        for (int i = 0; i < 9; i++) w->state.wpm[i] = w->state.wpm[i + 1];
        w->state.wpm[9] = st.wpm;
        redraw(w->obj, &w->state);
    }
}
static struct wpm_status_state wpm_get(const zmk_event_t *eh) {
    (void)eh;
    return (struct wpm_status_state){.wpm = zmk_wpm_get_state()};
}
ZMK_DISPLAY_WIDGET_LISTENER(widget_wpm_status, struct wpm_status_state, wpm_cb, wpm_get);
ZMK_SUBSCRIPTION(widget_wpm_status, zmk_wpm_state_changed);
int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_obj_set_style_bg_color(widget->obj, LVGL_BACKGROUND, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(widget->obj, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_pad_all(widget->obj, 0, LV_PART_MAIN);
    lv_obj_t *cv = lv_canvas_create(widget->obj);
    lv_canvas_set_buffer(cv, widget->cbuf, CANVAS_W, CANVAS_H, CANVAS_COLOR_FORMAT);
    lv_obj_align(cv, LV_ALIGN_TOP_LEFT, 0, 0);
    for (int i = 0; i < 10; i++) widget->state.wpm[i] = 0;
    widget->state.battery = zmk_battery_state_of_charge();
    widget->state.charging = zmk_usb_is_powered();
    widget->state.selected_endpoint = zmk_endpoint_get_selected();
    widget->state.active_profile_index = zmk_ble_active_profile_index();
    widget->state.active_profile_connected = zmk_ble_active_profile_is_connected();
    widget->state.active_profile_bonded = !zmk_ble_active_profile_is_open();
    widget->state.layer_index = zmk_keymap_highest_layer_active();
    widget->state.layer_label = zmk_keymap_layer_name(widget->state.layer_index);
    sys_slist_append(&widgets, &widget->node);
    redraw(widget->obj, &widget->state);
    widget_battery_status_init();
    widget_layer_status_init();
    widget_output_status_init();
    widget_wpm_status_init();
    return 0;
}
lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }

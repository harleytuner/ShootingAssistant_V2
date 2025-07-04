#include "ui_battery_indicator.h"
#include "BAT_Driver.h" // For BAT_Get_Volts()
#include <stdio.h>

static lv_obj_t *battery_container;
static lv_obj_t *battery_icon_label;
static lv_obj_t *battery_text_label;

// --- New Function: More accurate percentage calculation for LiPo ---
static int get_lipo_percentage(float voltage) {
    int percentage = 0;
    // This is a more realistic mapping based on a typical LiPo discharge curve.
    // Values are approximate and can be fine-tuned.
    if (voltage >= 4.20) {
        percentage = 100;
    } else if (voltage >= 4.10) {
        percentage = 90 + (int)((voltage - 4.10) / 0.10 * 10);
    } else if (voltage >= 4.00) {
        percentage = 80 + (int)((voltage - 4.00) / 0.10 * 10);
    } else if (voltage >= 3.90) {
        percentage = 70 + (int)((voltage - 3.90) / 0.10 * 10);
    } else if (voltage >= 3.80) {
        percentage = 60 + (int)((voltage - 3.80) / 0.10 * 10);
    } else if (voltage >= 3.70) {
        percentage = 45 + (int)((voltage - 3.70) / 0.10 * 15);
    } else if (voltage >= 3.60) {
        percentage = 20 + (int)((voltage - 3.60) / 0.10 * 25);
    } else if (voltage >= 3.50) {
        percentage = 10 + (int)((voltage - 3.50) / 0.10 * 10);
    } else if (voltage >= 3.30) {
        percentage = 5 + (int)((voltage - 3.30) / 0.20 * 5);
    } else {
        percentage = 0;
    }

    if (percentage > 100) percentage = 100;
    if (percentage < 0) percentage = 0;
    
    return percentage;
}
// -----------------------------------------------------------------


void ui_battery_indicator_create(lv_obj_t *parent) {
    battery_container = lv_obj_create(parent);
    lv_obj_set_size(battery_container, 80, 25);
    lv_obj_align(battery_container, LV_ALIGN_TOP_RIGHT, -5, 5);
    lv_obj_set_style_bg_opa(battery_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(battery_container, 0, 0);
    
    lv_obj_add_flag(battery_container, LV_OBJ_FLAG_FLOATING);
    lv_obj_clear_flag(battery_container, LV_OBJ_FLAG_CLICKABLE);

    battery_icon_label = lv_label_create(battery_container);
    lv_label_set_text(battery_icon_label, LV_SYMBOL_BATTERY_FULL);
    lv_obj_align(battery_icon_label, LV_ALIGN_RIGHT_MID, 0, 0);

    battery_text_label = lv_label_create(battery_container);
    lv_label_set_text(battery_text_label, "100%");
    lv_obj_align_to(battery_text_label, battery_icon_label, LV_ALIGN_OUT_LEFT_MID, -5, 0);
}

void ui_battery_indicator_update(void) {
    float voltage = BAT_Get_Volts();
    // --- Use the new function for percentage calculation ---
    int percentage = get_lipo_percentage(voltage);
    // -----------------------------------------------------

    char buf[8];
    sprintf(buf, "%d%%", percentage);
    lv_label_set_text(battery_text_label, buf);

    if (percentage > 80) {
        lv_label_set_text(battery_icon_label, LV_SYMBOL_BATTERY_FULL);
    } else if (percentage > 50) {
        lv_label_set_text(battery_icon_label, LV_SYMBOL_BATTERY_3);
    } else if (percentage > 20) {
        lv_label_set_text(battery_icon_label, LV_SYMBOL_BATTERY_2);
    } else if (percentage > 5) {
        lv_label_set_text(battery_icon_label, LV_SYMBOL_BATTERY_1);
    } else {
        lv_label_set_text(battery_icon_label, LV_SYMBOL_BATTERY_EMPTY);
    }
}
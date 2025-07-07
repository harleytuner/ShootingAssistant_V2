#include "ui_battery_indicator.h"
#include "BAT_Driver.h" // Needed for the BAT_analogVolts variable
#include <stdio.h>

static lv_obj_t *battery_container;
static lv_obj_t *battery_icon_label;
static lv_obj_t *battery_text_label;
// static lv_obj_t *voltage_text_label; // --- (1) REMOVE THIS LINE ---

// --- get_lipo_percentage function remains the same ---
static int get_lipo_percentage(float voltage) {
    int percentage = 0;
    if (voltage >= 4.15) {
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


void ui_battery_indicator_create(lv_obj_t *parent) {
    battery_container = lv_obj_create(parent);
    // --- (2) Change width back to 80 ---
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
    
    // --- (3) REMOVE THE VOLTAGE LABEL CREATION ---
    // voltage_text_label = lv_label_create(battery_container);
    // lv_label_set_text(voltage_text_label, "0.00V");
    // lv_obj_align_to(voltage_text_label, battery_text_label, LV_ALIGN_OUT_LEFT_MID, -5, 0);
}

void ui_battery_indicator_update(void) {
    float voltage = BAT_analogVolts;
    int percentage = get_lipo_percentage(voltage);

    char percent_buf[8];
    sprintf(percent_buf, "%d%%", percentage);
    lv_label_set_text(battery_text_label, percent_buf);

    // --- (4) REMOVE THE VOLTAGE LABEL UPDATE ---
    // char voltage_buf[8];
    // sprintf(voltage_buf, "%.2fV", voltage);
    // lv_label_set_text(voltage_text_label, voltage_buf);
    
    // Update the battery icon based on percentage
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
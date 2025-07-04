#ifndef UI_BATTERY_INDICATOR_H
#define UI_BATTERY_INDICATOR_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

void ui_battery_indicator_create(lv_obj_t *parent);
void ui_battery_indicator_update(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*UI_BATTERY_INDICATOR_H*/
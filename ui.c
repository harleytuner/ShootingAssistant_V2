// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.2
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"
#include "ui_battery_indicator.h"

///////////////////// VARIABLES ////////////////////

// EVENTS
lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    LV_EVENT_GET_COMP_CHILD = lv_event_register_id();

    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Options_Page_screen_init();
    ui_Bubble_Level_Page_screen_init();
    ui_Artificial_Horizon_Page_screen_init();
    ui_Shot_Counter_Page_screen_init();
    ui_Stage_Timer_Page_screen_init();
    ui_Competition_Page_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Options_Page);

    // Create the battery indicator on the top layer AFTER all screens are initialized
    ui_battery_indicator_create(lv_layer_top());
}

void ui_destroy(void)
{
    ui_Options_Page_screen_destroy();
    ui_Bubble_Level_Page_screen_destroy();
    ui_Artificial_Horizon_Page_screen_destroy();
    ui_Shot_Counter_Page_screen_destroy();
    ui_Stage_Timer_Page_screen_destroy();
    ui_Competition_Page_screen_destroy();
}
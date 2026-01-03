/*
 * FILE: ui.c
 * AUTHOR: LVGL Visual Architect (Generated)
 * LVGL Version: 8.4
 * DESCRIPTION: Implementation of the UI. This file should NOT be modified
 *              by the user, as it will be regenerated.
 */

#include "ui.h"
// #include "ui_i18n.h" // Assuming i18n is used
#include <string.h>
#include <Arduino.h>

/////////////////////
// STYLES
/////////////////////
void ui_styles_init(void)
{
    lv_disp_t *disp = lv_disp_get_default();
    /// static lv_theme_t *th = lv_theme_basic_init(disp); looks bad

    lv_disp_set_theme(
        disp,
        lv_theme_default_init(
            disp,
            lv_palette_main(LV_PALETTE_BLUE), // primary palette
            lv_palette_main(LV_PALETTE_CYAN), // secondary palette
            true,                             // dark mode enabled
            LV_FONT_DEFAULT));
}

// EVENT FORWARD DECLARATIONS
/////////////////////
void on_nav_bar_Main_Screen_Clicked(lv_event_t *e);
void on_nav_bar_Local_Screen_Clicked(lv_event_t *e);
void on_nav_bar_Screen_2_Clicked(lv_event_t *e);
void on_nav_bar_Service_Screen_Clicked(lv_event_t *e);

void on_button0_Clicked(lv_event_t *e);
void slider_event_cb(lv_event_t *e);

/////////////////////
// INTERNAL variables
/////////////////////

/////////////////////
// ASSETS
/////////////////////
// No image assets were included in this project.

/////////////////////
// SCREEN DEFINITIONS
/////////////////////
ui_screen_def_t UI_Screens;

/////////////////////
// WIDGET DEFINITIONS
/////////////////////

lv_obj_t *nav_bar;

/////////////////////

void create_navigation_button(lv_obj_t *parent, const char *label_text, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs, lv_event_cb_t event_cb)
{
    lv_obj_t *button = lv_btn_create(parent);
    lv_obj_set_size(button, lv_pct(25), 50);
    lv_obj_align(button, align, x_ofs, y_ofs);
    // lv_obj_set_style_bg_color(button, lv_color_hex(0x1f2937), 0);
    lv_obj_set_style_border_width(button, 0, 0);

    // Add label to button
    lv_obj_t *btn_label = lv_label_create(button);
    lv_label_set_text(btn_label, label_text);
    // lv_obj_set_style_text_font(btn_label, &lv_font_montserrat_30, 0);
    lv_obj_set_style_text_font(btn_label, &lv_font_montserrat_30, 0);
    lv_obj_center(btn_label);

    // Add click event listener to button
    lv_obj_add_event_cb(button, event_cb, LV_EVENT_CLICKED, NULL);
}

void create_navigation_bar(lv_obj_t *parent)
{
    create_navigation_button(parent, "HOME", LV_ALIGN_BOTTOM_LEFT, 0, 0, on_nav_bar_Main_Screen_Clicked);
    create_navigation_button(parent, "LOCALS", LV_ALIGN_BOTTOM_LEFT, lv_pct(25), 0, on_nav_bar_Local_Screen_Clicked);
    create_navigation_button(parent, "FORECAST", LV_ALIGN_BOTTOM_LEFT, lv_pct(50), 0, on_nav_bar_Screen_2_Clicked);
    create_navigation_button(parent, "SERVICE", LV_ALIGN_BOTTOM_LEFT, lv_pct(75), 0, on_nav_bar_Service_Screen_Clicked);
}

void ui_create_main_elements(lv_obj_t *parent)
{
    lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE);

    // Set background color to black
    lv_obj_set_style_bg_color(parent, lv_color_black(), 0);

    // Create navigation bar
    create_navigation_bar(parent);
}

/////////////////////
// SCREEN FUNCTIONS
/////////////////////

/////////////////////
// MAIN UI FUNCTION
/////////////////////
void ui_init(void)
{
    ui_styles_init();
    ui_Main_screen_init();
    ui_Local_screen_init();
    ui_Forecast_screen_init();
    ui_Service_Screen_init();
    lv_scr_load(UI_Screens.Main_Screen);
}
void ui_loop(void)
{
    if (Asset.UpdateUI_Screen == true)
    {
        Asset.UpdateUI_Screen = false;

        ui_Main_screen_update();
        ui_Local_screen_update();
        ui_Service_Screen_update();
        ui_Forecast_screen_update();
    }
}
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
void on_nav_bar_Screen_1_Clicked(lv_event_t *e);
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
lv_obj_t *value_label;
lv_obj_t *slider;

lv_obj_t *ui_spinbox1;
lv_obj_t *ui_status2;
lv_obj_t *ui_tabview4;
lv_obj_t *ui_spinner4;
lv_obj_t *ui_button7;
lv_obj_t *ui_label8;
lv_obj_t *ui_button5;
lv_obj_t *ui_inputfield6;
lv_obj_t *ui_label9;

const lv_style_selector_t DEFAULT_SELECTOR = (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT);
/////////////////////
lv_color_t lv_color_red(void)
{
    return lv_color_make(0xFF, 0x00, 0x00);
}

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
    create_navigation_button(parent, "MAIN", LV_ALIGN_BOTTOM_LEFT, 0, 0, on_nav_bar_Main_Screen_Clicked);
    create_navigation_button(parent, "Screen 1", LV_ALIGN_BOTTOM_LEFT, lv_pct(25), 0, on_nav_bar_Screen_1_Clicked);
    create_navigation_button(parent, "Screen 2", LV_ALIGN_BOTTOM_LEFT, lv_pct(50), 0, on_nav_bar_Screen_2_Clicked);
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
void ui_Main_screen_init(void);

void getBoolState();

void ui_Screen_1_screen_init(void)
{
    UI_Screens.Screen_1 = lv_obj_create(NULL);
    ui_create_main_elements(UI_Screens.Screen_1);

    ui_spinbox1 = lv_spinbox_create(UI_Screens.Screen_1);
    lv_obj_set_size(ui_spinbox1, 240, 114);
    lv_obj_align(ui_spinbox1, LV_ALIGN_TOP_LEFT, 166, 136);
    lv_spinbox_set_range(ui_spinbox1, -20, 50);
    lv_spinbox_set_value(ui_spinbox1, 22);
    ui_status2 = lv_label_create(UI_Screens.Screen_1);
    lv_label_set_text(ui_status2, LV_SYMBOL_WIFI);
    lv_obj_set_size(ui_status2, 29, 52);
    lv_obj_align(ui_status2, LV_ALIGN_TOP_LEFT, 270, 300);
    ui_tabview4 = lv_tabview_create(UI_Screens.Screen_1, LV_DIR_TOP, 50);
    lv_obj_set_size(ui_tabview4, 800, 40);
    lv_obj_align(ui_tabview4, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_t *ui_tabview4_Tab_1 = lv_tabview_add_tab(ui_tabview4, "Tab 1");

    ui_spinner4 = lv_spinner_create(UI_Screens.Screen_1, 1000, 60);
    lv_obj_set_size(ui_spinner4, 100, 50);
    lv_obj_align(ui_spinner4, LV_ALIGN_TOP_LEFT, 390, 330);

    ui_label8 = lv_label_create(UI_Screens.Screen_1);
    lv_label_set_text(ui_label8, "Screen 1");
    lv_obj_set_width(ui_label8, 226);
    lv_obj_set_height(ui_label8, 53);
    lv_obj_set_x(ui_label8, 513);
    lv_obj_set_y(ui_label8, 107);
    lv_obj_set_style_text_color(ui_label8, lv_color_white(), 0);
    lv_obj_set_style_text_font(ui_label8, &lv_font_montserrat_30, DEFAULT_SELECTOR);

    slider = lv_slider_create(UI_Screens.Screen_1);
    lv_obj_set_width(slider, 600);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, 80);
    lv_slider_set_range(slider, 0, 100);
    lv_slider_set_value(slider, 50, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    value_label = lv_label_create(UI_Screens.Screen_1);
    lv_label_set_text_fmt(value_label, "%d%%", lv_slider_get_value(slider));
    lv_obj_set_style_text_font(value_label, &lv_font_montserrat_30, 0);
    lv_obj_set_style_text_color(value_label, lv_color_hex(0xFF0000), 0);
    lv_obj_align(value_label, LV_ALIGN_CENTER, 0, 150);

    lv_obj_t *btn = lv_btn_create(UI_Screens.Screen_1);
    lv_obj_set_size(btn, 200, 50);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Click Me");
    lv_obj_set_style_text_font(btn_label, &lv_font_montserrat_30, LV_PART_MAIN); // for the button text
    lv_obj_center(btn_label);
    lv_obj_add_event_cb(btn, [](lv_event_t *e)
                        { Serial.println("🎉 Button clicked!"); }, LV_EVENT_CLICKED, NULL);
}

void ui_Screen_2_screen_init(void)
{
    UI_Screens.Screen_2 = lv_obj_create(NULL);
    ui_create_main_elements(UI_Screens.Screen_2);

    ui_inputfield6 = lv_textarea_create(UI_Screens.Screen_2);
    lv_obj_set_size(ui_inputfield6, 235, 50);
    lv_obj_align(ui_inputfield6, LV_ALIGN_TOP_LEFT, 181, 55);
    lv_textarea_set_one_line(ui_inputfield6, true);
    lv_textarea_set_text(ui_inputfield6, "InputField");

    ui_label9 = lv_label_create(UI_Screens.Screen_2);
    lv_label_set_text(ui_label9, "Screen 2");
    lv_obj_set_width(ui_label9, 430);
    lv_obj_set_height(ui_label9, 50);
    lv_obj_set_x(ui_label9, 19);
    lv_obj_set_y(ui_label9, 5);
    lv_obj_set_style_text_color(ui_label9, lv_color_white(), 0);
    lv_obj_set_style_text_font(ui_label9, &lv_font_montserrat_30, DEFAULT_SELECTOR);
}

// forward declaration
void ui_Service_Screen_init(void);
void ui_Service_Screen_update(void);

/////////////////////
// MAIN UI FUNCTION
/////////////////////
void ui_init(void)
{
    ui_styles_init();
    ui_Main_screen_init();
    ui_Screen_1_screen_init();
    ui_Screen_2_screen_init();
    ui_Service_Screen_init();
    lv_scr_load(UI_Screens.Main_Screen);
}
 void ui_loop(void)
 {
     // Placeholder for any periodic UI updates if needed
     ui_Service_Screen_update();
 }
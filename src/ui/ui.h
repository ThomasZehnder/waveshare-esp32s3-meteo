#pragma once

#include "lvgl.h"
#include "asset.h"

enum active_screen_t
{
    SCREEN_ID_Main_Screen,
    SCREEN_ID_Local_Screen,
    SCREEN_ID_Screen_2,
    SCREEN_ID_Service_Screen
};
struct ui_screen_def_t
{
    lv_obj_t *Main_Screen;
    lv_obj_t *Local_Screen;
    lv_obj_t *Screen_2;
    lv_obj_t *Service_Screen;

    active_screen_t active_screen;
};

extern ui_screen_def_t UI_Screens;

// main elements
void ui_create_main_elements(lv_obj_t *parent);

// Font declarations

// Screen externs

// Widget externs
extern lv_obj_t *value_label;
extern lv_obj_t *slider;

// Function declarations for event callbacks
void ui_init(void);
void ui_loop(void);

void ui_Service_Screen_init(void);
void ui_Service_Screen_update(void);
void ui_Main_screen_init(void);
void ui_Main_screen_update(void);
void ui_Local_screen_init(void);
void ui_Local_screen_update(void);

// forward
void getMeteoString(char *buf, size_t bufsize, const char *prefix, const meteo_t &meteo);

const lv_style_selector_t DEFAULT_SELECTOR = (lv_style_selector_t)(LV_PART_MAIN | LV_STATE_DEFAULT);

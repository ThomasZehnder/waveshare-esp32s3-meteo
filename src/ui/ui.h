/*
 * FILE: ui.h
 * AUTHOR: LVGL Visual Architect (Generated)
 * DESCRIPTION: Header file for the generated UI.
 *              Declares UI elements and the main initialization function.
 */

#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"

    // Asset declarations
    extern int click_count;

    // Font declarations

    // Screen externs

    extern lv_obj_t *ui_Main_Screen;
    extern lv_obj_t *ui_Screen_1;
    extern lv_obj_t *ui_Screen_2;
    extern lv_obj_t *ui_Service_Screen;

    extern lv_obj_t *counter_label;

    extern lv_obj_t *value_label;
    extern lv_obj_t *slider;

    // Widget externs

    // Function declarations for event callbacks
    void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*UI_H*/

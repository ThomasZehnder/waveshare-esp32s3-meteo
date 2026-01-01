#include <stdio.h>
#include <Arduino.h>

#include "ui.h"

    
// Button click event callback
void on_nav_bar_Main_Screen_Clicked(lv_event_t *e)
{
    UI_Screens.active_screen = SCREEN_ID_Main_Screen;
    lv_scr_load(UI_Screens.Main_Screen);
}

void on_nav_bar_Local_Screen_Clicked(lv_event_t *e)
{
    UI_Screens.active_screen = SCREEN_ID_Local_Screen;
    lv_scr_load(UI_Screens.Local_Screen);
}

void on_nav_bar_Screen_2_Clicked(lv_event_t *e)
{
    UI_Screens.active_screen = SCREEN_ID_Screen_2;
    lv_scr_load(UI_Screens.Forecast_Screen);
}

void on_nav_bar_Service_Screen_Clicked(lv_event_t *e)
{
    UI_Screens.active_screen = SCREEN_ID_Service_Screen;
    lv_scr_load(UI_Screens.Service_Screen);
}

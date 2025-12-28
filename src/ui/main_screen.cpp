#include "ui.h"
#include <string.h>
#include <Arduino.h>
#include "asset.h"

lv_obj_t *ui_0_button0;
lv_obj_t *ui_0_counter_label0;
lv_obj_t *ui_0_button1;
lv_obj_t *ui_0_counter_label1;

String getOnOffState(int count)
{

    if (count % 2 == 1)
    {
        return "on";
    }

    return "off";
}

void getText4Count(int x,int count, char *buf, int len)
{
    snprintf(buf, len, "Count %i: %d : %s", x, count, getOnOffState(count).c_str());
}

void on_0_button0_Clicked(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        Asset.clickCount1++;
        char buf[32];
        getText4Count(0, Asset.clickCount1, buf, sizeof(buf));
        printf("Button 0 clicked! : %s\n", buf);

        // Update counter label
        if (ui_0_counter_label0 != NULL)
        {
            lv_label_set_text(ui_0_counter_label0, buf);
        }
        Asset.sendLamp1Command = getOnOffState(Asset.clickCount1);
    }
}

void on_0_button1_Clicked(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        Asset.clickCount2++;

        char buf[32];
        getText4Count(1, Asset.clickCount2, buf, sizeof(buf));
        printf("Button 1 clicked! : %s\n", buf);

        // Update counter label
        if (ui_0_counter_label1 != NULL)
        {
            
            lv_label_set_text(ui_0_counter_label1, buf);
        }
        Asset.sendLamp2Command = getOnOffState(Asset.clickCount2);
    }
}

void ui_Main_screen_init(void)
{
    UI_Screens.Main_Screen = lv_obj_create(NULL);

    ui_create_main_elements(UI_Screens.Main_Screen);

    // Create a label on the screen
    lv_obj_t *label = lv_label_create(UI_Screens.Main_Screen);
    lv_label_set_text(label, "Main Screen");
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 10, 10);

    // Create single button in center of active screen
    ui_0_button0 = lv_btn_create(UI_Screens.Main_Screen);
    lv_obj_align(ui_0_button0, LV_ALIGN_TOP_LEFT, 10, 40);
    // Add click event listener to button
    lv_obj_add_event_cb(ui_0_button0, on_0_button0_Clicked, LV_EVENT_CLICKED, NULL);
    // Add label to button
    lv_obj_t *btn_label0 = lv_label_create(ui_0_button0);
    lv_label_set_text(btn_label0, "Lamp Office Thomas");
    lv_obj_set_style_text_font(btn_label0, &lv_font_montserrat_30, 0);
    lv_obj_center(btn_label0);

    // Create counter display label below button
    ui_0_counter_label0 = lv_label_create(UI_Screens.Main_Screen);
    lv_label_set_text(ui_0_counter_label0, "Count 0: 0");
    lv_obj_set_style_text_color(ui_0_counter_label0, lv_color_white(), 0);
    // Increase font size by using next available larger font
    lv_obj_set_style_text_font(ui_0_counter_label0, &lv_font_montserrat_30, 0);
    lv_obj_align(ui_0_counter_label0, LV_ALIGN_TOP_RIGHT, -10, 40);

    // Create second button below the first
    ui_0_button1 = lv_btn_create(UI_Screens.Main_Screen);
    lv_obj_align(ui_0_button1, LV_ALIGN_TOP_LEFT, 10, 120); // Position below first button
    // Add click event listener to button
    lv_obj_add_event_cb(ui_0_button1, on_0_button1_Clicked, LV_EVENT_CLICKED, NULL);
    // Add label to button
    lv_obj_t *btn_label1 = lv_label_create(ui_0_button1);
    lv_label_set_text(btn_label1, "Lamp Wohnzimmer");
    lv_obj_set_style_text_font(btn_label1, &lv_font_montserrat_30, 0);
    lv_obj_center(btn_label1);

    // Create counter display label for second button
    ui_0_counter_label1 = lv_label_create(UI_Screens.Main_Screen);
    lv_label_set_text(ui_0_counter_label1, "Count 1: 0");
    lv_obj_set_style_text_color(ui_0_counter_label1, lv_color_white(), 0);
    lv_obj_set_style_text_font(ui_0_counter_label1, &lv_font_montserrat_30, 0);
    lv_obj_align(ui_0_counter_label1, LV_ALIGN_TOP_RIGHT, -10, 120);
}

void ui_Main_screen_update(void)
{
    // Update counter labels
    if (ui_0_counter_label0 != NULL)
    {
        char buf[32];
        getText4Count(0, Asset.clickCount1, buf, sizeof(buf));
        lv_label_set_text(ui_0_counter_label0, buf);
    }
    if (ui_0_counter_label1 != NULL)
    {
        char buf[32];
        getText4Count(1, Asset.clickCount2, buf, sizeof(buf));
        lv_label_set_text(ui_0_counter_label1, buf);
    }
}
#include "ui.h"
#include <string.h>
#include <Arduino.h>
#include "asset.h"

lv_obj_t *ui_0_button0;
lv_obj_t *ui_0_counter_label0;
lv_obj_t *ui_0_button1;
lv_obj_t *ui_0_counter_label1;

lv_obj_t *ui_0_button_temp_up;
lv_obj_t *ui_0_button_temp_down;
lv_obj_t *ui_0_temperature_label;

String getOnOffState(int count)
{

    if (count % 2 == 1)
    {
        return "on";
    }

    return "off";
}

void getText4Count(int x, int count, char *buf, int len)
{
    snprintf(buf, len, "Count %i: %d : %s", x, count, getOnOffState(count).c_str());
}

void on_0_button0_Clicked(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        Asset.clickCount1++;
        Asset.UpdateUI_Screen = true;
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
        Asset.UpdateUI_Screen = true;

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

void on_0_button_temp_up_Clicked(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        Serial.println("Temperature UP button clicked!");
        // Increase temperature by 1 degree
        Asset.room1_settemperature += 1;
        Asset.sendTemperatureCommand = "sendMqtt";
        Asset.UpdateUI_Screen = true;
    }
}
void on_0_button_temp_down_Clicked(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        Serial.println("Temperature DOWN button clicked!");
        // Decrease temperature by 1 degree
        Asset.room1_settemperature -= 1;
        Asset.sendTemperatureCommand = "sendMqtt";
        Asset.UpdateUI_Screen = true;
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

    // Buttons to increase and decrease temperature
    ui_0_button_temp_down = lv_btn_create(UI_Screens.Main_Screen);
    lv_obj_align(ui_0_button_temp_down, LV_ALIGN_TOP_LEFT, 10, 200);
    lv_obj_t *btn_label_temp_down = lv_label_create(ui_0_button_temp_down);
    lv_label_set_text(btn_label_temp_down, "Temp -");
    lv_obj_set_style_text_font(btn_label_temp_down, &lv_font_montserrat_30, 0);
    lv_obj_center(btn_label_temp_down);
    lv_obj_add_event_cb(ui_0_button_temp_down, on_0_button_temp_down_Clicked, LV_EVENT_CLICKED, NULL);

    ui_0_button_temp_up = lv_btn_create(UI_Screens.Main_Screen);
    lv_obj_align(ui_0_button_temp_up, LV_ALIGN_TOP_LEFT, 10 + 200, 200);
    lv_obj_t *btn_label_temp_up = lv_label_create(ui_0_button_temp_up);
    lv_label_set_text(btn_label_temp_up, "Temp +");
    lv_obj_set_style_text_font(btn_label_temp_up, &lv_font_montserrat_30, 0);
    lv_obj_center(btn_label_temp_up);
    lv_obj_add_event_cb(ui_0_button_temp_up, on_0_button_temp_up_Clicked, LV_EVENT_CLICKED, NULL);

    // create label to show temperature
    ui_0_temperature_label = lv_label_create(UI_Screens.Main_Screen);
    lv_label_set_text(ui_0_temperature_label, "Temp: --.- C");
    lv_obj_set_style_text_color(ui_0_temperature_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(ui_0_temperature_label, &lv_font_montserrat_30, 0);
    lv_obj_align(ui_0_temperature_label, LV_ALIGN_TOP_RIGHT, -10, 200);
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
    if (ui_0_temperature_label != NULL)
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "Office Thomas: %.1f °C", Asset.room1_settemperature);
        lv_label_set_text(ui_0_temperature_label, buf);
    }
}
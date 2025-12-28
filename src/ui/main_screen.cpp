#include "ui.h"
#include <string.h>
#include <Arduino.h>
#include "asset.h"

lv_obj_t *ui_button0;


void on_button0_Clicked(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        click_count++;
        printf("Button clicked! Count: %d\n", click_count);

        // Update counter label
        if (counter_label != NULL)
        {
            char buf[32];
            snprintf(buf, sizeof(buf), "Count: %d", click_count);
            lv_label_set_text(counter_label, buf);
        }
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
    ui_button0 = lv_btn_create(UI_Screens.Main_Screen);
    lv_obj_align(ui_button0, LV_ALIGN_CENTER, 0, 0);

    // Add click event listener to button
    lv_obj_add_event_cb(ui_button0, on_button0_Clicked, LV_EVENT_CLICKED, NULL);

    // Add label to button
    lv_obj_t *btn_label = lv_label_create(ui_button0);
    lv_label_set_text(btn_label, "Click Me!");
    lv_obj_set_style_text_font(btn_label, &lv_font_montserrat_30, 0);
    lv_obj_center(btn_label);

    // Create counter display label below button
    counter_label = lv_label_create(UI_Screens.Main_Screen);
    lv_label_set_text(counter_label, "Count: 0");
    lv_obj_set_style_text_color(counter_label, lv_color_white(), 0);
    // Increase font size by using next available larger font
    lv_obj_set_style_text_font(counter_label, &lv_font_montserrat_30, 0);
    lv_obj_align(counter_label, LV_ALIGN_CENTER, 0, 60);

    // add spinner
    lv_obj_t *spinner = lv_spinner_create(UI_Screens.Main_Screen, 1000, 60);
    lv_color_t spinner_blue = lv_color_hex(0x1E90FF); // DodgerBlue
    lv_obj_set_style_arc_color(spinner, spinner_blue, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(spinner, lv_color_hex(0xFF0000), LV_PART_MAIN);
    lv_obj_align(spinner, LV_ALIGN_TOP_RIGHT, 0, 60);
    Serial.println("✅ Spinner created");
}
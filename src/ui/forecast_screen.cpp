#include "ui.h"
#include <Arduino.h>


lv_obj_t *value_label;
lv_obj_t *slider;

void slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);
    Serial.printf("Slider value: %d\n", value);
}


void ui_Forecast_screen_init(void)
{
    UI_Screens.Forecast_Screen = lv_obj_create(NULL);
    ui_create_main_elements(UI_Screens.Forecast_Screen);

    lv_obj_t *ui_inputfield6 = lv_textarea_create(UI_Screens.Forecast_Screen);
    lv_obj_set_size(ui_inputfield6, 235, 50);
    lv_obj_align(ui_inputfield6, LV_ALIGN_TOP_LEFT, 181, 55);
    lv_textarea_set_one_line(ui_inputfield6, true);
    lv_textarea_set_text(ui_inputfield6, "InputField");

    lv_obj_t *ui_label9 = lv_label_create(UI_Screens.Forecast_Screen);
    lv_label_set_text(ui_label9, "Screen 2");
    lv_obj_set_width(ui_label9, 430);
    lv_obj_set_height(ui_label9, 50);
    lv_obj_set_x(ui_label9, 19);
    lv_obj_set_y(ui_label9, 5);
    lv_obj_set_style_text_color(ui_label9, lv_color_white(), 0);
    lv_obj_set_style_text_font(ui_label9, &lv_font_montserrat_30, DEFAULT_SELECTOR);

     slider = lv_slider_create(UI_Screens.Forecast_Screen);
    lv_obj_set_width(slider, 600);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, 80);
    lv_slider_set_range(slider, 0, 100);
    lv_slider_set_value(slider, 50, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    value_label = lv_label_create(UI_Screens.Forecast_Screen);
    lv_label_set_text_fmt(value_label, "%d%%", lv_slider_get_value(slider));
    lv_obj_set_style_text_font(value_label, &lv_font_montserrat_30, 0);
    lv_obj_set_style_text_color(value_label, lv_color_hex(0xFF0000), 0);
    lv_obj_align(value_label, LV_ALIGN_CENTER, 0, 150);

    lv_obj_t *btn = lv_btn_create(UI_Screens.Forecast_Screen);
    lv_obj_set_size(btn, 200, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Click Me");
    lv_obj_set_style_text_font(btn_label, &lv_font_montserrat_30, LV_PART_MAIN); // for the button text
    lv_obj_center(btn_label);
    lv_obj_add_event_cb(btn, [](lv_event_t *e)
                        { Serial.println("🎉 Button clicked!"); }, LV_EVENT_CLICKED, NULL);
}

void ui_Forcast_screen_update(void)
{
    // Update slider value label
    if (value_label != NULL && slider != NULL)
    {
        lv_label_set_text_fmt(value_label, "%d%%", lv_slider_get_value(slider));
    }
}   

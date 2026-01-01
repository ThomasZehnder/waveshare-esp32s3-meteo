#include "ui.h"
#include <Arduino.h>
#include "asset.h"


lv_obj_t *ui_1_label0;
lv_obj_t *ui_1_label1;
lv_obj_t *ui_1_label2;

lv_obj_t *value_label;
lv_obj_t *slider;

void slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);
    Serial.printf("Slider value: %d\n", value);
}

void getValueString(char *buf, size_t bufsize, const char* prefix, const meteo_t &meteo)
{
    snprintf(buf, bufsize, "%s: Temp: %.2f°C, Hum: %.2f %%", prefix, meteo.temperature, meteo.humidity);
}


void ui_Local_screen_init(void)
{
    UI_Screens.Local_Screen = lv_obj_create(NULL);
    ui_create_main_elements(UI_Screens.Local_Screen);

    ui_1_label0 = lv_label_create(UI_Screens.Local_Screen);
    lv_label_set_text(ui_1_label0, "Esp Sensor");
    lv_obj_align(ui_1_label0, LV_ALIGN_TOP_LEFT, 0, 0);


    ui_1_label1 = lv_label_create(UI_Screens.Local_Screen);
    lv_label_set_text(ui_1_label1, "Inside Values ");
    lv_obj_align(ui_1_label1, LV_ALIGN_TOP_LEFT, 0, 40);
    lv_obj_set_style_text_font(ui_1_label1, &lv_font_montserrat_30, DEFAULT_SELECTOR);

    ui_1_label2 = lv_label_create(UI_Screens.Local_Screen);
    lv_label_set_text(ui_1_label2, "Outside Values ");
    lv_obj_align(ui_1_label2, LV_ALIGN_TOP_LEFT, 0, 80);
    lv_obj_set_style_text_font(ui_1_label2, &lv_font_montserrat_30, DEFAULT_SELECTOR);

       slider = lv_slider_create(UI_Screens.Local_Screen);
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

    lv_obj_t *btn = lv_btn_create(UI_Screens.Local_Screen);
    lv_obj_set_size(btn, 200, 50);
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Click Me");
    lv_obj_set_style_text_font(btn_label, &lv_font_montserrat_30, LV_PART_MAIN); // for the button text
    lv_obj_center(btn_label);
    lv_obj_add_event_cb(btn, [](lv_event_t *e)
                        { Serial.println("🎉 Button clicked!"); }, LV_EVENT_CLICKED, NULL);
}

void ui_Local_screen_update(void)
{
    char buf[128];
    getValueString(buf, sizeof(buf), "Innen" , Asset.inside);
    lv_label_set_text(ui_1_label1, buf);    
    getValueString(buf, sizeof(buf), "Aussen" , Asset.outside);
    lv_label_set_text(ui_1_label2, buf);

}
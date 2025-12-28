#include <Arduino.h>
#include <lvgl.h>
#include "board/init.h"

static lv_obj_t *value_label;

void slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int value = lv_slider_get_value(slider);
    lv_label_set_text_fmt(value_label, "%d%%", value);
    Serial.printf("Slider value: %d\n", value);
}

void setup()
{
    board_init();

    Serial.println("Creating LVGL widgets...");

    lv_obj_clean(lv_scr_act());
    lv_obj_t *scr = lv_scr_act();
    lv_obj_set_style_bg_color(scr, lv_color_make(0, 0, 0), 0);
    Serial.println("✅ Screen background set to black");

    lv_obj_t *title = lv_label_create(scr);
    lv_label_set_text(title, "WAVESHARE_ESP32_S3_TOUCH_LCD_4_3 Demo");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_30, LV_PART_MAIN);  // e.g., for your percent label
    Serial.println("✅ Title label created");

    lv_obj_t *slider = lv_slider_create(scr);
    lv_obj_set_width(slider, 600);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, -30);
    lv_slider_set_range(slider, 0, 100);
    lv_slider_set_value(slider, 50, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    Serial.println("✅ Slider created");

    value_label = lv_label_create(scr);
    lv_label_set_text(value_label, "50%");
    lv_obj_set_style_text_font(value_label, &lv_font_montserrat_30, LV_PART_MAIN);  // e.g., for your percent label
    lv_obj_align(value_label, LV_ALIGN_CENTER, 0, 60);
    lv_obj_set_style_text_color(value_label, lv_color_hex(0xFF0000), 0);
    Serial.println("✅ Value label created");

    lv_obj_t *btn = lv_btn_create(scr);
    lv_obj_set_size(btn, 200, 50);
    lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_obj_t *btn_label = lv_label_create(btn);
    lv_label_set_text(btn_label, "Click Me");
    lv_obj_set_style_text_font(btn_label,   &lv_font_montserrat_30, LV_PART_MAIN);  // for the button text
    lv_obj_center(btn_label);
    lv_obj_add_event_cb(btn, [](lv_event_t * e){ Serial.println("🎉 Button clicked!"); }, LV_EVENT_CLICKED, NULL);
    Serial.println("✅ Button created");

    //add spinner
    lv_obj_t *spinner = lv_spinner_create(scr, 1000, 60);
    lv_color_t spinner_blue = lv_color_hex(0x1E90FF);  // DodgerBlue
    lv_obj_set_style_arc_color(spinner, spinner_blue, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(spinner, lv_color_hex(0xFF0000), LV_PART_MAIN);
    lv_obj_align(spinner, LV_ALIGN_TOP_RIGHT, 0, 60);
    Serial.println("✅ Spinner created");

    lv_refr_now(NULL);
    Serial.println("✅ Screen refresh forced");
    Serial.println("Setup complete!");
}

void loop()
{
    static uint32_t last_tick = millis();

    uint32_t now = millis();
    lv_tick_inc(now - last_tick);
    last_tick = now;

    lv_timer_handler();

    delay(5);
}
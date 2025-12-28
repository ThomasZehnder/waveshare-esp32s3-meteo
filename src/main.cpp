#include <Arduino.h>
#include <lvgl.h>
#include <Wire.h>
#include "board/diagnostics.h"
#include "board/touch.h"
#include "board/lcd.h"
#include "board/BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3.h"

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf1 = nullptr;
static lv_color_t *buf2 = nullptr;
static lv_obj_t *value_label;
static lv_indev_drv_t indev_drv;
static bool touch_detected = false;

void slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    int value = lv_slider_get_value(slider);
    lv_label_set_text_fmt(value_label, "%d%%", value);
    Serial.printf("Slider value: %d\n", value);
}

void setup()
{
    Serial.begin(115200);

    if (!psramFound()) {
        Serial.println("❌ PSRAM NOT FOUND");
        while (1) delay(1000);
    }

    Serial.printf("✅ PSRAM OK: %d bytes\n", ESP.getPsramSize());

    pinMode(LCD_BL, OUTPUT);
    digitalWrite(LCD_BL, HIGH);
    Serial.println("Backlight ON");

    // Initialize GT911 Touch Controller
    Serial.println("Initializing GT911 touch controller...");
    touch_detected = touch_init(LCD_H_RES, LCD_V_RES);
    if (!touch_detected) {
        Serial.println("⚠️ GT911 init failed, but enabling touch for testing...");
        touch_detected = true;
    }
    Serial.printf("Touch system status: %s\n", touch_detected ? "ENABLED" : "DISABLED");

    /*
    Serial.println("\n*** Please touch the screen now for manual test ***");
    delay(2000);
    manual_touch_test();
    */

    lv_init();

    // Initialize LCD and LVGL display driver
    if (!lcd_init_display(&draw_buf, &buf1, &buf2)) {
        while (1) delay(1000);
    }

    // Reinitialize I2C after display setup to fix GT911 communication
    Serial.println("🔄 Reinitializing I2C for GT911 after display setup...");
    Wire.end();
    delay(100);
    Wire.begin(touch_get_sda(), touch_get_scl());
    Wire.setClock(100000);
    delay(100);

    Wire.beginTransmission(touch_get_default_addr());
    int error = Wire.endTransmission();
    if (error == 0) {
        Serial.printf("✅ GT911 still responding at 0x%02X after I2C reinit\n", touch_get_default_addr());
    } else {
        Serial.printf("❌ GT911 not responding after reinit! Error=%d\n", error);
    }

    // Register touch input with proper configuration
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    indev_drv.user_data = NULL;
    lv_indev_t *touch_indev = lv_indev_drv_register(&indev_drv);
    if (touch_indev) {
        Serial.println("✅ Touch input device registered successfully");
        Serial.printf("Touch device type: %d, read_cb: %p\n", indev_drv.type, indev_drv.read_cb);
    } else {
        Serial.println("❌ Failed to register touch input device");
    }

    // Force an immediate touch read test
    Serial.println("Testing immediate touchpad_read call...");
    lv_indev_data_t test_data;
    touchpad_read(&indev_drv, &test_data);
    Serial.printf("Test result: state=%d, x=%d, y=%d\n", test_data.state, test_data.point.x, test_data.point.y);

    // Force LVGL to start processing input immediately
    Serial.println("Forcing LVGL input refresh...");
    lv_timer_handler();

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
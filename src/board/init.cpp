#include <Arduino.h>
#include <Wire.h>
#include "board/diagnostics.h"
#include "board/touch.h"
#include "board/lcd.h"
#include "board/BOARD_WAVESHARE_ESP32_S3_TOUCH_LCD_4_3.h"



// NOTE:
// These LVGL-related objects must have static storage duration because
// LVGL keeps internal references to them after initialization:
//  - lcd_init_display() stores pointers to draw_buf and the underlying
//    color buffers (buf1, buf2) and uses them during rendering.
//  - lv_indev_drv_register() stores a pointer to indev_drv and uses it
//    asynchronously for input handling.
// They therefore cannot be moved into board_init()'s local scope without
// causing dangling references inside LVGL.
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf1 = nullptr;
static lv_color_t *buf2 = nullptr;
static lv_indev_drv_t indev_drv;

void board_init()
{
    Serial.begin(115200);
    Serial.println("Board initialization started.");

    if (!psramFound())
    {
        Serial.println("❌ PSRAM NOT FOUND");
        while (1)
            delay(1000);
    }

    Serial.printf("✅ PSRAM OK: %d bytes\n", ESP.getPsramSize());

    // Initialize backlight
    pinMode(LCD_BL, OUTPUT);
    digitalWrite(LCD_BL, HIGH);
    Serial.println("✅ Backlight initialized and turned ON");

    /*
    static bool touch_detected = false;
    Serial.println("Initializing GT911 touch controller...");
    touch_detected = touch_init(LCD_H_RES, LCD_V_RES);
    if (!touch_detected)
    {
        Serial.println("⚠️ GT911 init failed, but enabling touch for testing...");
        touch_detected = true;
    }
    Serial.printf("Touch system status: %s\n", touch_detected ? "ENABLED" : "DISABLED");
        /*
    Serial.println("\n*** Please touch the screen now for manual test ***");
    delay(2000);
    manual_touch_test();
    */

    // Initialize LCD and LVGL display driver
    lv_init();
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

    // Initialize the GT911 now that I2C has been reconfigured
    bool touch_ok = touch_init(LCD_H_RES, LCD_V_RES);
    Serial.printf("Touch init result: %s\n", touch_ok ? "OK" : "FAILED");

    // Register touch input with proper configuration
    if (touch_ok) {
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
    } else {
        Serial.println("⚠️ Skipping LVGL touch registration because GT911 init failed");
    }

    // Force an immediate touch read test
    Serial.println("Testing immediate touchpad_read call...");
    lv_indev_data_t test_data;
    touchpad_read(&indev_drv, &test_data);
    Serial.printf("Test result: state=%d, x=%d, y=%d\n", test_data.state, test_data.point.x, test_data.point.y);

    // Force LVGL to start processing input immediately
    Serial.println("Forcing LVGL input refresh...");
    lv_timer_handler();

    Serial.println("🎉 Board initialization complete.");
}
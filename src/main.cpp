#include <Arduino.h>
#include <lvgl.h>
#include "board/init.h"
#include "ui/ui.h"

void setup()
{
    board_init();

    // Initialize LVGL screens
    ui_init();

    //lv_refr_now(NULL);

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
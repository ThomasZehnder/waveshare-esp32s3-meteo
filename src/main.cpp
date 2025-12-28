#include <Arduino.h>
#include <lvgl.h>
#include "board/init.h"
#include "ui/ui.h"
#include "wlan/wlan.h"
#include "asset.h"

asset_t Asset;

void setup()
{
    board_init();

    // Initialize LVGL screens
    ui_init();

    wlan_init(); //WIFI and MQTT

    Serial.println("Setup complete!");
}

void loop()
{
    static uint32_t last_tick = millis();

    uint32_t now = millis();
    lv_tick_inc(now - last_tick);
    last_tick = now;

    lv_timer_handler();

    wlan_loop();

    ui_loop();
    
    delay(5);
}
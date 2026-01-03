#include <Arduino.h>
#include <lvgl.h>
#include "board/init.h"
#include "ui/ui.h"
#include "wlan/wlan.h"
#include "wlan/mqtt.h"
#include "meteo/nodesky-meteo.h"
#include "datetime.h"
#include "asset.h"

asset_t Asset;

void setup()
{
    unsigned long start = millis();
    Serial.printf("Setup started at %lu ms\n", start);

    board_init();
    Serial.printf("Board init done at %lu ms\n", millis() - start);

    // Initialize LVGL screens
    ui_init();
    Serial.printf("UI init done at %lu ms\n", millis() - start);

    wlan_init(); // WIFI and MQTT
    Serial.printf("WLAN init done at %lu ms\n", millis() - start);

    setupNTP(); // NTP time sync
    Serial.printf("NTP setup done at %lu ms\n", millis() - start);

    meteo_init();
    Serial.printf("Meteo init done at %lu ms\n", millis() - start);

    Serial.printf("Total setup time: %lu ms\n", millis() - start);
    Serial.println("Setup complete!");
}

void loop()
{
    static uint32_t last_task_time = 0;
    static uint32_t last_tick = millis();

    uint32_t now = millis();
    lv_tick_inc(now - last_tick);
    last_tick = now;

    lv_timer_handler();

    // Schedule tasks every 500ms
    if (now - last_task_time >= 500) {
        lv_timer_handler();
        wlan_loop();
        ui_loop();
        updateDatetime(); // Update datetime
        last_task_time = now;
    }

    mqtt_loop();

    meteo_loop();

    delay(5);
}
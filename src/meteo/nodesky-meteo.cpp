#include "meteo/nodesky-meteo.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "asset.h"
#include "wlan/credentials.h"

static const char* URL = NODESKY_URL NODESKY_OUTSIDE_DEVICE_ID;
static uint32_t last_fetch_time = 0;
static const uint32_t FETCH_INTERVAL = 10000; // 10 seconds

void meteo_init() {
    // Initialization if needed
    last_fetch_time = millis() + FETCH_INTERVAL; // Fetch immediately on first loop
}

void meteo_loop() {
    uint32_t now = millis();
    if (now - last_fetch_time >= FETCH_INTERVAL) {
        last_fetch_time = now;

        HTTPClient http;
        http.begin(URL);
        int httpCode = http.GET();

        if (httpCode > 0) {
            String payload = http.getString();
            Serial.println("HTTP Response: " + payload);

            // Parse JSON
            DynamicJsonDocument doc(2048);
            DeserializationError error = deserializeJson(doc, payload);

            if (error) {
                Serial.print("JSON parse error: ");
                Serial.println(error.c_str());
            } else {
                if (doc.is<JsonArray>() && doc.size() > 0) {
                    JsonObject jsonData = doc[0]["jsonData"];
                    if (jsonData.containsKey("temperature")) {
                        Asset.outside.temperature = jsonData["temperature"];
                    }
                    if (jsonData.containsKey("humidity")) {
                        Asset.outside.humidity = jsonData["humidity"];
                    }
                    Serial.printf("Updated outside temp: %.2f, humidity: %.2f\n", Asset.outside.temperature, Asset.outside.humidity);
                }
            }
        } else {
            Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
}

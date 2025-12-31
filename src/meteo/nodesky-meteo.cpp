#include <Arduino.h>
#include "meteo/nodesky-meteo.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "asset.h"
#include "wlan/credentials.h"

static int getState = 0;

static const char *URL_OUTSIDE = NODESKY_URL NODESKY_OUTSIDE_DEVICE_ID;
static const char *URL_INSIDE = NODESKY_URL NODESKY_INSIDE_DEVICE_ID;

static uint32_t last_fetch_time = 0;
static const uint32_t FETCH_INTERVAL = 10000; // 10 seconds

void storeMeteo(meteo_t &meteo, DynamicJsonDocument &doc)
{
    if (doc.is<JsonArray>() && doc.size() > 0)
    {
        JsonObject jsonData = doc[0]["jsonData"];
        if (jsonData.containsKey("temperature"))
        {
            meteo.temperature = jsonData["temperature"];
        }
        if (jsonData.containsKey("humidity"))
        {
            meteo.humidity = jsonData["humidity"];
        }
        if (jsonData.containsKey("devicename"))
        {
            meteo.deviceName = jsonData["devicename"].as<String>();
        }
        Serial.printf("Updated %s temp: %.2f, humidity: %.2f\n", Asset.outside.deviceName.c_str(), Asset.outside.temperature, Asset.outside.humidity);
    }
}

void meteo_init()
{
    // Initialization if needed
    last_fetch_time = -10 * FETCH_INTERVAL; // Fetch immediately on first loop
}

void meteo_loop()
{
    uint32_t now = millis();
    if (now - last_fetch_time >= FETCH_INTERVAL)
    {
        last_fetch_time = now;

        String URL = String(URL_OUTSIDE);
        getState = (getState + 1) % 2;
        if (getState == 0)
        {
            URL = String(URL_OUTSIDE);
        }
        else if (getState == 1)
        {
            URL = String(URL_INSIDE);
        }

        HTTPClient http;
        http.begin(URL);
        int httpCode = http.GET();

        if (httpCode > 0)
        {
            String payload = http.getString();
            Serial.println("HTTP Response: " + payload);

            // Parse JSON
            DynamicJsonDocument doc(2048);
            DeserializationError error = deserializeJson(doc, payload);

            if (error)
            {
                Serial.print("JSON parse error: ");
                Serial.println(error.c_str());
            }
            else
            {

                if (getState == 0)
                {
                    storeMeteo(Asset.outside, doc);
                }
                else if (getState == 1)
                {
                    storeMeteo(Asset.outside, doc);
                }
            }
        }
        else
        {
            Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
}

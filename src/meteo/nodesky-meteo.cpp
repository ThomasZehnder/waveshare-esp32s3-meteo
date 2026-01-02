#include <Arduino.h>
#include "meteo/nodesky-meteo.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "asset.h"
#include "credentials.h"



static const char *URL_OUTSIDE = NODESKY_URL NODESKY_OUTSIDE_DEVICE_ID;
static const char *URL_INSIDE = NODESKY_URL NODESKY_INSIDE_DEVICE_ID;

static const char *OPEN_METEO_URL = "https://api.open-meteo.com/v1/forecast?latitude=47.4615&longitude=9.0455&timezone=auto&daily=temperature_2m_min,temperature_2m_max,precipitation_sum,wind_speed_10m_max&forecast_days=";


static int getState = 0; // 0: outside, 1: inside, 2: open-meteo
static uint32_t last_fetch_time = 0;
static uint32_t fetch_intervall = 500;
static const uint32_t FETCH_INTERVAL = 10000; // 10 seconds
static bool first_run = true;

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
        Asset.UpdateUI_Screen = true;
    }
}

void storeForecastMeteo(DynamicJsonDocument &doc)
{
    // Implement storing forecast data if needed
    /*{
  "latitude": 47.46,
  "longitude": 9.039999,
  "generationtime_ms": 0.151991844177246,
  "utc_offset_seconds": 3600,
  "timezone": "Europe/Zurich",
  "timezone_abbreviation": "GMT+1",
  "elevation": 571,
  "daily_units": {
    "time": "iso8601",
    "temperature_2m_min": "°C",
    "temperature_2m_max": "°C",
    "precipitation_sum": "mm",
    "wind_speed_10m_max": "km/h"
  },
  "daily": {
    "time": [
      "2026-01-01",
      "2026-01-02",
      "2026-01-03",
      "2026-01-04",
      "2026-01-05"
    ],
    "temperature_2m_min": [-8, -2.6, -4.3, -10.9, -12.9],
    "temperature_2m_max": [1.3, 0.3, -0.9, -2.6, -3.8],
    "precipitation_sum": [0, 0.7, 2.2, 0, 0],
    "wind_speed_10m_max": [15.9, 20.4, 17, 11, 6.5]
  }
}*/
    // For now, just print that we received the data
    Serial.println("Forecast data received.");
    Asset.forecast = forecast_t(); // Reset to default values   
    for (int i = 0; i < NUM_FORECAST_DAYS; i++) {
        Asset.forecast.time[i] = doc["daily"]["time"][i].as<String>();
        Asset.forecast.temperature_2m_min[i] = doc["daily"]["temperature_2m_min"][i];
        Asset.forecast.temperature_2m_max[i] = doc["daily"]["temperature_2m_max"][i];
        Asset.forecast.precipitation_sum[i] = doc["daily"]["precipitation_sum"][i];
        Asset.forecast.wind_speed_10m_max[i] = doc["daily"]["wind_speed_10m_max"][i];
    }   
    Asset.UpdateUI_Screen = true;
}

void meteo_init()
{
    // Initialization if needed
    first_run = true;
}

void meteo_loop()
{
    uint32_t now = millis();
    if (first_run || now - last_fetch_time >= fetch_intervall)
    {
        first_run = false;
        last_fetch_time = now;

        String URL = "";
        if (getState == 0)
        {
            URL = String(URL_OUTSIDE);
        }
        else if (getState == 1)
        {
            URL = String(URL_INSIDE);
        }
        else if (getState == 2)
        {
            URL = String(OPEN_METEO_URL) + String(NUM_FORECAST_DAYS);
            fetch_intervall = FETCH_INTERVAL; // reset to normal interval after first two quick fetches
        }
        Serial.println("HTTP Fetch: " + URL);

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
                    storeMeteo(Asset.inside, doc);
                }
                else if (getState == 2)
                {
                    // Handle Open-Meteo data if needed
                    storeForecastMeteo(doc);
                    Serial.println("Open-Meteo data received.");
                }

            }
        }
        else
        {
            Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
            getState = 0; // reset state machine on error
        }

        http.end();
        getState = (getState + 1) % 3;
    }
}

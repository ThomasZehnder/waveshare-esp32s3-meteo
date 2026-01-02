#pragma once

#include "Arduino.h"

#define NUM_FORECAST_DAYS 5 

struct meteo_t
{
    String deviceName = "???";
    float temperature = -111.0;
    float humidity = -111.0;
};

struct forecast_t
{

    float temperature_2m_min[NUM_FORECAST_DAYS] = {-111.0, -111.0, -111.0, -111.0, -111.0};
    float temperature_2m_max[NUM_FORECAST_DAYS] = {-111.0, -111.0, -111.0, -111.0, -111.0};
    float precipitation_sum[NUM_FORECAST_DAYS] = {-111.0, -111.0, -111.0, -111.0, -111.0};
    float wind_speed_10m_max[NUM_FORECAST_DAYS] = {-111.0, -111.0, -111.0, -111.0, -111.0};
    String time[NUM_FORECAST_DAYS] = {"", "", "", "", ""};
};

struct datetime_t
{
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;
    String formatted = "??";
};

struct asset_t
{
    String deviceId = "ESP32S3_Meteo_001";
    String ipAddr = "???.???.???.???";
    bool wifiConnected= 0;
    bool mqttConnected= 0;

    String sendLamp1Command = "";
    String sendLamp2Command = "";

    int clickCount1 = 0;
    int clickCount2 = 0;

    float room1_settemperature = -111.0; //default value
    String sendTemperatureCommand = "";

    // weather data
    meteo_t outside;
    meteo_t inside;

    forecast_t forecast;

    // datetime
    datetime_t datetime;

    bool UpdateUI_Screen = false;
};

extern asset_t Asset;
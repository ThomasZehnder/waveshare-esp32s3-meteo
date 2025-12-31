#pragma once

struct meteo_t
{
    float temperature = -111.0;
    float humidity = -111.0;
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

    bool UpdateUI_Screen = false;
};

extern asset_t Asset;
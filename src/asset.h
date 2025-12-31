#pragma once

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

    bool UpdateUI_Screen = false;
};

extern asset_t Asset;
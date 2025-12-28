#pragma once

struct asset_t
{
    String deviceId = "ESP32S3_Meteo_001";
    String ipAddr = "???.???.???.???";
    bool wifiConnected= 0;
    bool mqttConnected= 0;
};

extern asset_t Asset;
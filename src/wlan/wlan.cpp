#include <Arduino.h>
#include <WiFi.h>
#include "wlan.h"
#include "mqtt.h"


#include "credentials.h"
#include "asset.h"

void onWifiConnect(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("onWifiConnect (CallBack) --> Connected to WiFi... " + WiFi.SSID());
    Serial.print("onWifiConnect (CallBack) --> IP address: ");
    Serial.println(WiFi.localIP());
    Asset.ipAddr = WiFi.localIP().toString();
    Asset.wifiConnected = true;

    // mqttClient.setServer(ip, Assembly.cfg.mqtt[cfgIndex].port);
    // Serial.print("onWifiConnect (CallBack) --> mqtt client id: ");
    // Serial.println(mqttClient.getClientId());

    // connectToMqtt();
}

void onWifiDisconnect(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("MqttSetup (CallBack) --> Disconnected from WiFi.");
    Asset.wifiConnected = false;

    // mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
    //  wifiReconnectTimer.once(2, connectToWifi);
}

void wlan_init()
{
    Serial.println("WLAN Setup --> Start");

    // connect to wifi
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("WLAN Setup --> Connecting to WiFi SSID: ");
    Serial.println(WIFI_SSID);

    WiFi.onEvent(onWifiConnect, ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(onWifiDisconnect, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    Serial.println("WLAN Setup --> End");
}

void wlan_loop()
{
    // nothing to do here for now
}

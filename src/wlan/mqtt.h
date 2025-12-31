#pragma once

void mqtt_connect();
void mqtt_disconnect();
void mqtt_loop();   



void mqttPublishLong(const char* topic, long x);
void mqttPublishReal(const char* topic, float x);
void mqttPublishString(const char* topic, String s);
void mqttPublishJson(const char* topic, String s);

void mqttSubscribe (const char *topic);
void mqttUnsubscribe (const char *topic);
bool mqttCheckTopic (const char *subscribedTopic, const char *receivedTopic);
#include <Arduino.h>
#include "mqtt.h"
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include "credentials.h"

#include "dictionary.h"
#include "../asset.h"

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

Dictionary<String, String> SubscriptionList;

#define ASSENMBLY_JOB_TOPIC "job"

void connectToMqtt()
{
  Serial.println("connectToMqtt (CallBack) --> Connecting to MQTT..." + String(MQTT_HOST));
  mqttClient.connect();
}

// MQTT event callbacks
void onMqttDisconnect(bool sessionPresent)
{
  Serial.println("onMqttDisconnect (CallBack) --> Connected to MQTT..." + String(MQTT_HOST));
  Asset.mqttConnected = true;
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  /*   uint16_t packetIdSub = mqttClient.subscribe("test/lol", 2);
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.println(packetIdSub);
    mqttClient.publish("test/lol", 0, true, "test 1");
    Serial.println("Publishing at QoS 0");
    uint16_t packetIdPub1 = mqttClient.publish("test/lol", 1, true, "test 2");
    Serial.print("Publishing at QoS 1, packetId: ");
    Serial.println(packetIdPub1);
    uint16_t packetIdPub2 = mqttClient.publish("test/lol", 2, true, "test 3");
    Serial.print("Publishing at QoS 2, packetId: ");
    Serial.println(packetIdPub2); */

  mqttPublishString("myIpAddr", Asset.ipAddr);


  // subscribe assembly "job"
  mqttSubscribe(ASSENMBLY_JOB_TOPIC);
}
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  Serial.println("onMqttDisconnect (CallBack) --> Disconnected from MQTT.");
  Asset.mqttConnected = false;

  if (Asset.wifiConnected)
  {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId)
{
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
  // interpretate payload as zeroended string
  char s[255];
  memset(s, 0, sizeof(s));
  size_t l = len;
  if (len > sizeof(s) - 1)
  {
    l = sizeof(s) - 1;
  }
  memcpy(s, payload, l);

  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  payload: ");
  Serial.println(s); // as zeroended string
  Serial.print("  qos: ");
  Serial.println(properties.qos);
  Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);

  // set received topic to list
  SubscriptionList.set(topic, String(s));

  // check on job, fixe defined
  if (mqttCheckTopic(ASSENMBLY_JOB_TOPIC, topic))
  {
    Serial.println("  --> JOB received: " + String(s) + " - " + topic);
  }
}

void onMqttPublish(uint16_t packetId)
{
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void mqttSubscribe(const char *topic)
{
  String t;

    t = Asset.deviceId;
    t += "/";
    t += topic;
  

  // uint16_t packetIdSub = mqttClient.subscribe(t.c_str(), 0);

  // check is subscription still done
  if (SubscriptionList.get(t) == "ndef")
  {

    uint16_t packetIdSub = mqttClient.subscribe(t.c_str(), 0);

    // add to list
    SubscriptionList.set(t, "new...");

    Serial.print("Add to list: Subscribing at QoS 0, packetId: ");
    Serial.print(packetIdSub);
    Serial.print(" topic: ");
    Serial.println(t);
  }
}

bool mqttCheckTopic(const char *topic, const char *inTopic)
{
  String t(Asset.deviceId);
  t += "/";
  t += topic;
  return (strcmp(t.c_str(), inTopic) == 0);
}

String _generateTopic(const char *topic)
{
  String t;

  t = Asset.deviceId;
  t += "/";
  t += topic;

  return t;
}
String _generateValue(const char *topic, String s)
{
  String v;

  v = s;

  return v;
}


void mqttPublishLong(const char *topic, long x)
{
  char s[200];
  ltoa(x, s, 10);
  String t = _generateTopic(topic);
  String v = _generateValue(topic, s);

  mqttClient.publish(t.c_str(), 0, true, v.c_str());
  // add to local list (only for test)
  // SubscriptionList.set(t,s);

  // Serial.println("Publishing long at QoS 0");
  Serial.print("Publish Long: ");
  Serial.print(t);
  Serial.print(" : ");
  Serial.println(x);
}
void mqttPublishString(const char *topic, String s)
{
  String t = _generateTopic(topic);
  String v = _generateValue(topic, String("\"") + s + String("\""));

  mqttClient.publish(t.c_str(), 0, true, v.c_str());
  // add to local list (only for test)
  // SubscriptionList.set(t,s);

  // Serial.println("Publishing String at QoS 0");
  Serial.print("Publish String: ");
  Serial.print(t);
  Serial.print(" : ");
  Serial.println(s);
}

void mqttPublishJson(const char *topic, String s)
{
  String t = _generateTopic(topic);
  String v = s;

  mqttClient.publish(t.c_str(), 0, true, v.c_str());
  // add to local list (only for test)
  // SubscriptionList.set(t,s);

  // Serial.println("Publishing String at QoS 0");
  Serial.print("Publish JSON: ");
  Serial.print(t);
  Serial.print(" : ");
  Serial.println(s);
}


void mqtt_connect()
{
    // Initialize MQTT client here
    Serial.println("MQTT connect.");

    // set callbactks, etc.
    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onSubscribe(onMqttSubscribe);
    mqttClient.onUnsubscribe(onMqttUnsubscribe);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.onPublish(onMqttPublish);

    // connect to MQTT Broker, etc.
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    if (MQTT_USERNAME && MQTT_PASSWORD)
    {
        mqttClient.setCredentials(MQTT_USERNAME, MQTT_PASSWORD);
    }
    mqttClient.connect();
}

void mqtt_disconnect()
{
    // Disconnect MQTT client here
    Serial.println("MQTT disconnect.");
}

void mqtt_loop()
{
    // Handle MQTT client loop here
}

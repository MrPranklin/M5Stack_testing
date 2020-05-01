#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <iostream>
#include <sstream>

#include "ServoBlindController.hpp"
#include "mqtt.hpp"
#include "MqttTopics.h"
#include "LedController.hpp"

#define SERVO_PIN 4
#define LED_PIN 12

void setupWifi();

const char *ssid = "Pranklin";
const char *password = "MrPranklin";
const char *host = "Nova_LightController";

const IPAddress mqttServerIp = IPAddress(192, 168, 0, 24);
const int mqttServerPort = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

ServoBlindController *servo;
LedController *led;

void setup() {
    Serial.begin(115200);

    servo = new ServoBlindController(SERVO_PIN, 180);
    led = new LedController(LED_PIN);
    setupWifi();
}

void loop() {

    if (!mqttClient.loop()) {
        Serial.println("MQTT disconnected");
        mqtt::reconnect(mqttClient);
    }
}

void callback(char *topic, byte *payload, unsigned int length) {
    payload[length] = '\0';
    Serial.print("MQTT: received ");
    Serial.print((char *) payload);
    Serial.print(" on ");
    Serial.println(topic);
    String strTopic = String((char *) topic);

    std::stringstream strValue;
    strValue << (char *) payload;

    int intValue;
    strValue >> intValue;

    if (strTopic == mqtt_command_blinds) {
        servo->setPercentage(intValue);
    } else if (strTopic == mqtt_command_light) {
        led->setPercentage(intValue);
    }
}

void setupWifi() {
    WiFi.begin(ssid, password);
    WiFi.hostname(host);
    Serial.println("");

    while (WiFi.isConnected()) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    WiFi.setAutoReconnect(true);

    Serial.println("Setting up MQTT");

    mqttClient.setServer(mqttServerIp, mqttServerPort);
    mqttClient.setCallback(callback);
    mqtt::reconnect(mqttClient);

    Serial.println("MQTT setup done");
}
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <sstream>

#include "ota.hpp"
#include "mqtt.hpp"
#include "Cooler.hpp"
#include "FanController.hpp"
#include "Heater.hpp"
#include "MockHeater.hpp"
#include "HeatControl.hpp"
#include "mqtt_topics.h"
#include "RelayHeater.hpp"

void setupWifi();

const char *ssid = "Pranklin";
const char *password = "MrPranklin";
const char *host = "Nova_HeatController";

const IPAddress mqttServerIp = IPAddress(192, 168, 0, 24);
const int mqttServerPort = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

HeatControl *heatControl;

void setup() {
    Serial.begin(115200);
    setupWifi();
    ota::begin();

    Cooler *c = new FanController(12);
    Heater *h = new RelayHeater(5, 10000);

    std::vector<Cooler *> coolers;
    coolers.push_back(c);

    std::vector<Heater *> heaters;
    heaters.push_back(h);

    heatControl = new HeatControl(coolers, heaters);
    heatControl->turnOnAllHeaters();

    Serial.println("Setup finished");
}

void loop() {
    ota::handleClient();

    if (!mqttClient.loop()) {
        Serial.println("MQTT disconnected");
        mqtt::reconnect(mqttClient);
    } else {
        heatControl->update();
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

    int intValue = 0;
    strValue >> intValue;

    if (strTopic == mqtt_command_cooling) {
        heatControl->setPercentageToAllCoolers(intValue);
    } else if (strTopic == mqtt_command_heating) {
        heatControl->setPercentageToAllHeaters(intValue);
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
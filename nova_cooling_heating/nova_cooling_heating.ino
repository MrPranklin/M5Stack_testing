#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

#include "ota.hpp"
#include "mqtt.hpp"
#include "Cooler.hpp"
#include "FanController.hpp"
#include "Heater.hpp"
#include "MockHeater.hpp"
#include "HeatControl.hpp"

void setupWifi();

const char *ssid = "Pranklin";
const char *password = "MrPranklin";
const char *host = "Nova_HeatController";

const IPAddress mqttServerIp = IPAddress(192, 168, 0, 104);
const int mqttServerPort = 1883;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

HeatControl *heatControl;

void setup() {
    Serial.begin(115200);
    setupWifi();
    ota::begin();

    Cooler *c = new FanController(12);
    Heater *h = new MockHeater(13);

    std::vector<Cooler*> coolers;
    coolers.push_back(c);

    std::vector<Heater*> heaters;
    heaters.push_back(h);

    heatControl = new HeatControl (&coolers, &heaters);

    Serial.println("Setup finished");
}

void loop() {
    ota::handleClient();

    if (!mqttClient.loop()) {
        Serial.println("MQTT disconnected");
        mqtt::reconnect(mqttClient);
    }
}

void callback(char *topic, byte *payload, unsigned int length) {
    payload[length] = '\0';
    String strTopic = String((char *) topic);
    String pyld = String((char *) payload);
    if (strTopic == "NOVA/cooling_in") {
        if (pyld == "ON") {
            Serial.println("cooling ON");
//            heatControl->turnOnAllCoolers();
//            mqtt::send_data(mqttClient, "NOVA/cooling_out", "ON");
        } else {
            Serial.println("cooling OFF");
//            heatControl->turnOffAllCoolers();
//            mqtt::send_data(mqttClient, "NOVA/cooling_out", "OFF");
        }
    } else if(strTopic == "NOVA/heating_in"){
        if (pyld == "ON") {
            Serial.println("heating ON");
//            heatControl->turnOnAllHeaters();
//            mqtt::send_data(mqttClient, "NOVA/heating_out", "ON");
        } else {
            Serial.println("heating OFF");
//            heatControl->turnOffAllHeaters();
//            mqtt::send_data(mqttClient, "NOVA/heating_out", "OFF");
        }
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
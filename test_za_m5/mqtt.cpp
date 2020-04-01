#include "mqtt.hpp"

#include <Arduino.h>
#include <stdlib.h>
#include "m5lcd.hpp"
#include "mqtt_topics.h"

const char *mqtt_out_topic = "M5/out";
const char *mqtt_out_topic_temp = "M5/temp";
const char *mqtt_out_topic_hum = "M5/hum";
const char *mqtt_client_id = "M5Stack_client";

#define INTERVAL 10000

namespace mqtt {
    bool send_data(PubSubClient client, const char *topic, const char *payload, bool retain) {
        bool status = client.publish(topic, payload, retain);
        if (status) {
            Serial.print("MQTT: published ");
            Serial.print(payload);
            Serial.print(" to ");
            Serial.println(topic);
        } else {
            Serial.println("MQTT: publish failed");
        }
        return status;
    }

    void reconnect(PubSubClient client) {
        while (!client.connected()) {
            Serial.print("Attempting MQTT connection...");
            m5lcd::showMessage("Connecting MQTT");

            if (client.connect(mqtt_client_id)) {
                Serial.print("connected as ");
                Serial.println(mqtt_client_id);
                // Once connected, publish an announcement...
                client.publish(mqtt_out_topic, "This is M5Stack");
                // ... and resubscribe
                if (client.subscribe(mqtt_command_heat_control)) {
                    Serial.print("Subscribed to ");
                    Serial.println(mqtt_command_heat_control);
                } else {
                    Serial.print("Failed to subscribe to ");
                    Serial.println(mqtt_command_heat_control);
                }

                m5lcd::clear();
            } else {
                Serial.print("failed, rc=");
                Serial.print(client.state());
                Serial.println(" try again in 5 seconds");
                // Wait 5 seconds before retrying
                delay(5000);
            }
        }
    }

    void publish_temperature(PubSubClient client, float temperature) {
        char output[10];
        dtostrf(temperature, 6, 2, output);
        send_data(client, mqtt_out_topic_temp, output, false);
    }

    void publish_humidity(PubSubClient client, float humidity) {
        char output[10];
        dtostrf(humidity, 6, 2, output);
        send_data(client, mqtt_out_topic_hum, output, false);
    }

    bool shouldUpdate(long millis, long lastMillis) {
        if (millis - lastMillis > INTERVAL) {
            return true;
        }
        return false;
    }

    void sendTurnOnCooling(PubSubClient client) {
        send_data(client, mqtt_command_cooling, "ON", true);
    }

    void sendTurnOffCooling(PubSubClient client) {
        send_data(client, mqtt_command_cooling, "OFF", true);
    }

    void sendTurnOnHeating(PubSubClient client) {
        send_data(client, mqtt_command_heating, "ON", true);
    }

    void sendTurnOffHeating(PubSubClient client) {
        send_data(client, mqtt_command_heating, "OFF", true);
    }

    void confirmHeatControlOn(PubSubClient client) {
        send_data(client, mqtt_state_heat_control, "ON", true);
    }

    void confirmHeatControlOff(PubSubClient client) {
        send_data(client, mqtt_state_heat_control, "OFF", true);
    }

    bool unsubscribe(PubSubClient client, const char *topic) {
        bool result = client.unsubscribe(topic);

        if (result) {
            Serial.print("Unsubscribed from: ");
            Serial.println(topic);
        } else {
            Serial.print("Failed to unsubscribe from: ");
            Serial.println(topic);
        }

        return result;
    }

};

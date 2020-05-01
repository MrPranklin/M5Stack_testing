#include <Arduino.h>
#include <cstdio>

#include "mqtt.hpp"
#include "m5lcd.hpp"
#include "mqtt_topics.h"

const char *mqtt_out_topic = "M5/out";
const char *mqtt_client_id = "M5Stack_light";

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
                client.publish(mqtt_out_topic, "This is M5Stack light");
                // ... and resubscribe
                if (client.subscribe(mqtt_command_light_control)) {
                    Serial.print("Subscribed to ");
                    Serial.println(mqtt_command_light_control);
                } else {
                    Serial.print("Failed to subscribe to ");
                    Serial.println(mqtt_command_light_control);
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

    bool shouldUpdate(long millis, long lastMillis) {
        return millis - lastMillis > INTERVAL;
    }

    void confirmLightControlOn(PubSubClient client) {
        send_data(client, mqtt_state_light_control, "ON", true);
    }

    void confirmLightControlOff(PubSubClient client) {
        send_data(client, mqtt_state_light_control, "OFF", true);
    }

    void updateArtificialLightPercentage(PubSubClient client, int percentage) {
        char buff[10];
        sprintf(buff, "%d", percentage);
        send_data(client, mqtt_command_artificial, buff, true);
    }

    void updateNaturalLightPercentage(PubSubClient client, int percentage) {
        char buff[10];
        sprintf(buff, "%d", percentage);
        send_data(client, mqtt_command_natural, buff, true);
    }

    bool unsubscribe(PubSubClient client, const char *topic) {
        bool result = client.unsubscribe(topic);

        if (result) {
            Serial.print("MQTT: Unsubscribed from: ");
            Serial.println(topic);
        } else {
            Serial.print("MQTT: Failed to unsubscribe from: ");
            Serial.println(topic);
        }

        return result;
    }
}

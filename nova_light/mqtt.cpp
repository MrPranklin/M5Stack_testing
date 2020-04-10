#include <Arduino.h>
#include <cstdlib>

#include "mqtt.hpp"
#include "MqttTopics.h"

const char *mqtt_status_topic = "NOVA/status/light";
const char *mqtt_client_id = "nova_light_control";

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

            if (client.connect(mqtt_client_id)) {
                Serial.print("connected as ");
                Serial.println(mqtt_client_id);
                // Once connected, publish an announcement...
                client.publish(mqtt_status_topic, "This is Nova_cooling_heating");
                // ... and resubscribe
                Serial.println("Subscribed to ");

                if (client.subscribe(mqtt_command_light)) {
                    Serial.print("    ");
                    Serial.println(mqtt_command_light);
                }

                if (client.subscribe(mqtt_command_blinds)) {
                    Serial.print("    ");
                    Serial.println(mqtt_command_blinds);
                }
            } else {
                Serial.print("failed, rc=");
                Serial.print(client.state());
                Serial.println(" try again in 5 seconds");
                // Wait 5 seconds before retrying
                delay(5000);
            }
        }
    }
}

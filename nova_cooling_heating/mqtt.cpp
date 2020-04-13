#include "mqtt.hpp"

#include <Arduino.h>
#include <cstdlib>
#include "mqtt_topics.h"

const char *mqtt_status_topic = "NOVA/status";
const char *mqtt_client_id = "nova_cooling_heating";

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
                client.subscribe(mqtt_command_heating);
                client.subscribe(mqtt_command_cooling);

                Serial.println("Subscribed to ");

                Serial.print("    "); Serial.println(mqtt_command_cooling);
                Serial.print("    "); Serial.println(mqtt_command_heating);
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

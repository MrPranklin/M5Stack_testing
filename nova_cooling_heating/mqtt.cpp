#include "mqtt.hpp"

#include <Arduino.h>
#include <cstdlib>
#include "mqtt_topics.h"

const char *mqtt_out_topic = "NOVA/out";
const char *mqtt_client_id = "nova_cooling_heating";

namespace mqtt {
    void send_data(PubSubClient client, const char *topic, const char *payload) {
        client.publish(topic, payload);
        Serial.print("MQTT: published "); Serial.print(payload); Serial.print(" to "); Serial.println(topic);
    }

    void reconnect(PubSubClient client) {
        while (!client.connected()) {
            Serial.print("Attempting MQTT connection...");

            if (client.connect(mqtt_client_id)) {
                Serial.print("connected as ");
                Serial.println(mqtt_client_id);
                // Once connected, publish an announcement...
                client.publish(mqtt_out_topic, "This is Nova_cooling_heating");
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

    void confirmCoolingOn(PubSubClient client){
        send_data(client, mqtt_state_cooling, "ON");
    }

    void confirmCoolingOff(PubSubClient client){
        send_data(client, mqtt_state_cooling, "OFF");
    }

    void confirmHeatingOn(PubSubClient client){
        send_data(client, mqtt_state_heating, "ON");
    }

    void confirmHeatingOff(PubSubClient client){
        send_data(client, mqtt_state_heating, "OFF");
    }
}

#include "mqtt.hpp"

#include <Arduino.h>
#include <cstdlib>

const char *mqtt_out_topic = "NOVA/out";
const char *mqtt_client_id = "nova_cooling_heating";

namespace mqtt {
    void send_data(PubSubClient client, const char *topic, const char *payload) {
        client.publish(topic, payload);
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
                client.subscribe("NOVA/cooling_in");
                client.subscribe("NOVA/heating_in");

                Serial.println("Subscribed to ");

                Serial.println("NOVA/cooling_in");
                Serial.println("NOVA/heating_in");
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

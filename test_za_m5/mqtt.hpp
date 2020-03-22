#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>

namespace mqtt {
void send_data(PubSubClient client, const char* topic, const char* payload);
void reconnect(PubSubClient client);
void publish_temperature(PubSubClient client, float temperature);
void publish_humidity(PubSubClient client, float humidity);
void callback(char* topic, byte* payload, unsigned int length);
};  // namespace mqtt

#endif

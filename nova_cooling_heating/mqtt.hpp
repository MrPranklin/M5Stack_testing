#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>

namespace mqtt {
    void send_data(PubSubClient client, const char *topic, const char *payload);

    void reconnect(PubSubClient client);

    void confirmCoolingOn(PubSubClient client);

    void confirmCoolingOff(PubSubClient client);

    void confirmHeatingOn(PubSubClient client);

    void confirmHeatingOff(PubSubClient client);
};  // namespace mqtt

#endif

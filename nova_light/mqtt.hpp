#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>

namespace mqtt {
    bool send_data(PubSubClient client, const char *topic, const char *payload, bool retain);

    void reconnect(PubSubClient client);
};  // namespace mqtt

#endif

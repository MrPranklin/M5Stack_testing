#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>

namespace mqtt {
    bool send_data(PubSubClient client, const char *topic, const char *payload, bool retain);

    void reconnect(PubSubClient client);

    bool shouldUpdate(long millis, long lastMillis);

    void updateArtificialLightPercentage(PubSubClient client, int percentage);

    void updateNaturalLightPercentage(PubSubClient client, int percentage);

    void confirmLightControlOn(PubSubClient client);

    void confirmLightControlOff(PubSubClient client);

    bool unsubscribe(PubSubClient client, const char *topic);
};  // namespace mqtt

#endif

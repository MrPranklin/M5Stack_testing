#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>

namespace mqtt {
    bool send_data(PubSubClient client, const char *topic, const char *payload);

    void reconnect(PubSubClient client);

    void publish_temperature(PubSubClient client, float temperature);

    void publish_humidity(PubSubClient client, float humidity);

    bool shouldUpdate(long millis, long lastMillis);

    void sendTurnOnCooling(PubSubClient client);

    void sendTurnOffCooling(PubSubClient client);

    void sendTurnOnHeating(PubSubClient client);

    void sendTurnOffHeating(PubSubClient client);

    void confirmHeatControlOn(PubSubClient client);

    void confirmHeatControlOff(PubSubClient client);

    void updateTargetTemp(PubSubClient client, float temp);

    bool unsubscribe(PubSubClient client, const char *topic);
};  // namespace mqtt

#endif

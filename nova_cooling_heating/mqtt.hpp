#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>

//const char *mqtt_in_cooling = "NOVA/cooling_in";
//const char *mqtt_in_heating = "NOVA/heating_in";
//const char *mqtt_out_heating_state = "NOVA/heating_out";
//const char *mqtt_out_cooling_state = "NOVA/cooling_out";

namespace mqtt {
    void send_data(PubSubClient client, const char *topic, const char *payload);

    void reconnect(PubSubClient client);
};  // namespace mqtt

#endif

#include "mqtt.hpp"

MQTT::MQTT(const IPAddress server, int port, ptr_cb callback, WiFiClient wifi_client){
    client = new PubSubClient(wifi_client);
    client->setServer(server, port);
    client->setCallback(callback);
}

MQTT::~MQTT(){
    delete client;
    client = NULL;
}

bool MQTT::connect(const char* client_id){
  return client->connect(client_id);  
}

bool MQTT::connected(){
    return client->connected();
}

bool MQTT::loop(){
    return client->loop();
}

int MQTT::state(){
    return client->state();
}

bool MQTT::subscribe(const char* topic){
    return client->subscribe(topic);
}

bool MQTT::unsubscribe(const char* topic){
    return client->unsubscribe(topic);
}

int MQTT::publish(const char* topic, const char* payload){
  client->publish(topic, payload);
  }

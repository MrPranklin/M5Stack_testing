#include <PubSubClient.h>
#include <WiFiClient.h>
typedef void (*ptr_cb)(char*, uint8_t*, unsigned int);

class MQTT {
   public:
    MQTT(const IPAddress server, int port, ptr_cb callback, WiFiClient wifi_client);
    ~MQTT();
    bool connected();
    bool connect(const char* client_id);
    bool loop();
    bool subscribe(const char* topic);
    bool unsubscribe(const char* topic);
    int state();
    int publish(const char* topic, const char* payload);

   private:
    PubSubClient* client;
};

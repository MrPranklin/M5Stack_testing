#include <DHT.h>

class DHT22_C
{
public:
    DHT22_C(short int pin);

    ~DHT22_C();

    void begin();

    float read_temperature();

    float read_humidity();

    bool is_sensor_ready(long interval);

private:
    long lastMillis = 0;
    DHT *dht = NULL;
};

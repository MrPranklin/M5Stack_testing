#ifndef DHT22_C_H
#define DHT22_C_H

#include <DHT.h>
#include "HumSensor/HumSensor.hpp"
#include "TempSensor/TempSensor.hpp"

class DHT22_C : HumSensor, TempSensor {
public:
    DHT22_C(short int pin);

    ~DHT22_C();

    void begin();

    float readTemperature();

    float readHumidity();

private:
    bool isSensorReady(long interval);

    float lastTemp;
    float lastHum;

    long lastMillis = 0;
    DHT *dht = NULL;
};

#endif

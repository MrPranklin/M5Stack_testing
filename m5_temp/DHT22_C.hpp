#ifndef DHT22_C_H
#define DHT22_C_H

#include <DHT.h>
#include "HumSensor.hpp"
#include "TempSensor.hpp"

class DHT22_C : public HumSensor, public TempSensor {
public:
    explicit DHT22_C(short int pin);

    ~DHT22_C() override;

    void begin();

    float readTemperature() override;

    float readHumidity() override;

private:
    bool isSensorReady(long interval);

    float lastTemp;
    float lastHum;

    long lastMillis = 0;
    DHT *dht = nullptr;
};
#endif

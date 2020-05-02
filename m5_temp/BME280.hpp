#ifndef BME280_H
#define BME280_H

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "HumSensor.hpp"
#include "TempSensor.hpp"

class BME280 : public HumSensor, public TempSensor {
public:

    ~BME280() override = default;

    unsigned begin();

    float readTemperature() override;

    float readHumidity() override;

private:
    Adafruit_BME280 bme;

    bool isSensorReady(long interval);

    float lastTemp{0};
    float lastHum{0};
    long lastMillis{0};
};

#endif //BME280_H
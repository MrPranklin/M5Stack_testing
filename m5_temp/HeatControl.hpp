#ifndef HEAT_CONTROL_H
#define HEAT_CONTROL_H

#include <PubSubClient.h>

#include "TempSensor.hpp"
#include "Cooler.hpp"
#include "Heater.hpp"
#include "DHT22_C.hpp"
#include <vector>

class HeatControl {
public:
    HeatControl(TempSensor *tempSensor, PubSubClient client);

    ~HeatControl();

    virtual void setTargetTemp(float temp);

    virtual bool update();

    virtual void enable();

    virtual void disable();

    virtual bool isEnabled();

    virtual float getTargetTemp();

    virtual float incrementTargetTemp(float amount);

    virtual int getCoolingPercentage();

    virtual int getHeatingPercentage();

    virtual bool setCoolingPercentage(int percentage);

    virtual bool setHeatingPercentage(int percentage);

protected:
    virtual float getCurrentTemp();

    TempSensor *_tempSensor;
    PubSubClient _client;
    bool _isEnabled{false};
    float _targetTemp{22};
    float _currentTemp{0};

    int _currentCoolingPercentage{0};
    int _currentHeatingPercentage{0};
};

#endif //HEAT_CONTROL_H

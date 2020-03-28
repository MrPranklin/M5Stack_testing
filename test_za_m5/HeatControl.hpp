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

    virtual void turnEverythingOff();

    virtual bool isEnabled();

protected:

    virtual void turnOnHeating();

    virtual void turnOffHeating();

    virtual void turnOnCooling();

    virtual void turnOffCooling();

    virtual float getCurrentTemp();

    TempSensor *_tempSensor;
    PubSubClient _client;
    bool _isEnabled;
    float _targetTemp;
    float _currentTemp;

    bool _isCoolingOn;
    bool _isHeatingOn;
};

#endif //HEAT_CONTROL_H

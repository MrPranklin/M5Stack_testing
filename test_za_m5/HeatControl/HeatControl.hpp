#ifndef HEAT_CONTROL_H
#define HEAT_CONTROL_H

#include "../TempSensor/TempSensor.hpp"
#include "../Cooler/Cooler.h"
#include "../Heater/Heater.hpp"

class HeatControl {
private:
    HeatControl(TempSensor *tempSensor, std::vector<Cooler *> coolers, std::vector<Heater *> heaters)

    ~HeatControl();

    virtual void turnOnHeater(Heater *h);

    virtual void turnOffHeater(Heater *h);

    virtual void turnOnAllHeaters();

    virtual void turnOffAllHeaters();

    virtual void turnOnCooler(Cooler *c);

    virtual void turnOffCooler(Cooler *c);

    virtual void turnOnAllCoolers();

    virtual void turnOffAllCoolers();

    virtual float getCurrentTemp(TempSensor *tempSensor);

    std::vector<Cooler *> coolers;
    std::vector<Heater *> heaters;
    TempSensor *tempSensor;
    bool isEnabled;
    float targetTemp;
    float currentTemp;

public:
    virtual void setTargetTemp(float temp);

    virtual bool update();

    virtual void enable();

    virtual void disable();

    virtual void turnEverythingOff();

    virtual bool isEnabled();
};

#endif HEAT_CONTROL_H
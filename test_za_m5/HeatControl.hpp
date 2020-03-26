#ifndef HEAT_CONTROL_H
#define HEAT_CONTROL_H

#include "TempSensor.hpp"
#include "Cooler.hpp"
#include "Heater.hpp"
#include "DHT22_C.hpp"
#include <vector>

class HeatControl {
public:
    HeatControl(DHT22_C *tempSensor, std::vector<Cooler *> *coolers, std::vector<Heater *> *heaters);

    ~HeatControl();

    virtual void setTargetTemp(float temp);

    virtual bool update();

    virtual void enable();

    virtual void disable();

    virtual void turnEverythingOff();

    virtual bool isEnabled();

protected:
    virtual void turnOnHeater(Heater *h);

    virtual void turnOffHeater(Heater *h);

    virtual void turnOnAllHeaters();

    virtual void turnOffAllHeaters();

    virtual void turnOnCooler(Cooler *c);

    virtual void turnOffCooler(Cooler *c);

    virtual void turnOnAllCoolers();

    virtual void turnOffAllCoolers();

    virtual float getCurrentTemp();

    std::vector<Cooler *> *_coolers;
    std::vector<Heater *> *_heaters;
    TempSensor *_tempSensor;
    bool _isEnabled;
    float _targetTemp;
    float _currentTemp;
};

#endif //HEAT_CONTROL_H

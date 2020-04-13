#ifndef HEAT_CONTROL_H
#define HEAT_CONTROL_H

#include "Cooler.hpp"
#include "Heater.hpp"
#include <vector>

class HeatControl {
public:
    HeatControl(std::vector<Cooler *> coolers, std::vector<Heater *> heaters);

    ~HeatControl();

    virtual void enable();

    virtual void disable();

    virtual void turnEverythingOff();

    virtual bool isEnabled();

    virtual void turnOnAllCoolers();

    virtual void turnOffAllCoolers();

    virtual void turnOnAllHeaters();

    virtual void turnOffAllHeaters();

    virtual void setPercentageToAllHeaters(int percentage);

    virtual void setPercentageToAllCoolers(int percentage);

protected:
    virtual void turnOnHeater(Heater *h);

    virtual void turnOffHeater(Heater *h);

    virtual void turnOnCooler(Cooler *c);

    virtual void turnOffCooler(Cooler *c);

    virtual void setPercentageToCooler(Cooler *c, int percentage);

    virtual void setPercentageToHeater(Heater *h, int percentage);

    std::vector<Cooler *> _coolers;
    std::vector<Heater *> _heaters;

    bool _isEnabled;
};

#endif //HEAT_CONTROL_H

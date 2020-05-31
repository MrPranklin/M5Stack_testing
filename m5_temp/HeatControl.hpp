#ifndef HEAT_CONTROL_H
#define HEAT_CONTROL_H

#include "TempSensor.hpp"
#include "Cooler.hpp"
#include "Heater.hpp"
#include "DHT22_C.hpp"
#include <vector>
#include <AutoPID.h>

class HeatControl {
public:
    explicit HeatControl(TempSensor *tempSensor);

    ~HeatControl();

    virtual void setTargetTemp(float temp);

    virtual void update();

    virtual void enable();

    virtual void disable();

    virtual bool isEnabled();

    virtual float getTargetTemp();

    virtual float incrementTargetTemp(float amount);

    virtual int getCoolingPercentage();

    virtual int getHeatingPercentage();

    virtual void setCoolingPercentage(int percentage);

    virtual void setHeatingPercentage(int percentage);

protected:
    virtual float getCurrentTemp();

    TempSensor *_tempSensor;
    AutoPID *_autopid;
    bool _isEnabled{false};
    float _targetTemp{22};
    double _targetTempDouble{22.};
    float _currentTemp{22};
    double _currentTempDouble{22.};

    int _currentCoolingPercentage{0};
    int _currentHeatingPercentage{0};
    double PID_output, PID_output_old;
};

#endif //HEAT_CONTROL_H

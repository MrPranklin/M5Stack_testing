#ifndef RELAYHEATER_H
#define RELAYHEATER_H

#include "Heater.hpp"

class RelayHeater: public Heater {
public:
    RelayHeater(int pin, long dutyCyclePeriod);

    void turnOn() override;

    void turnOff() override;

    void setPercentage(int percentage) override;

    void update() override;

private:

    void pwmOn();
    void pwmOff();
    void calcOnOffTime();

    int _pin;
    long _dutyCyclePeriod;
    long _onTime{0};
    long _offTime{10000};
    bool _pwmState{false};
    long _lastPwmMillis{0};
};

#endif //RELAYHEATER_H

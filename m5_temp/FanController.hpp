#ifndef FAN_CONTROLLER_H
#define FAN_CONTROLLER_H

#include "Cooler.hpp"

class FanController : public Cooler {
public:

    explicit FanController(int pin);

    void turnOn() override;

    void turnOff() override;

    bool isEnabled() override;

private:
    int _pin;
};

#endif //FAN_CONTROLLER_H

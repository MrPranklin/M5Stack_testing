#ifndef LIGHTCONTROLLER_H
#define LIGHTCONTROLLER_H

#include "Light.hpp"

class LedController : public Light {
public:
    LedController(int pin);

    void turnOn() override;

    void turnOff() override;

    bool toggle() override;

    bool isOn() override;

    void setPercentage(int percentage) override;

    int getPercentage() override;

protected:
    int _pin;
};

#endif // LIGHTCONTROLLER_H
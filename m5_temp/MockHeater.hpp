#ifndef MOCK_HEATER_H
#define MOCK_HEATER_H

#include "Heater.hpp"

class MockHeater : public Heater {
public:
    MockHeater(int pin);

    void turnOn();

    void turnOff();

    bool isEnabled();

private:
    int _pin;
};

#endif //MOCK_HEATER_H

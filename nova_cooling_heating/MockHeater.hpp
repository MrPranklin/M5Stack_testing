#ifndef MOCK_HEATER_H
#define MOCK_HEATER_H

#include "Heater.hpp"

class MockHeater : public Heater {
public:
    explicit MockHeater(int pin);

    void turnOn() override ;

    void turnOff() override ;

    bool isEnabled() override ;

    void setPercentage(int percentage) override ;

    int getPercentage() override ;

private:
    int _pin;
};

#endif //MOCK_HEATER_H

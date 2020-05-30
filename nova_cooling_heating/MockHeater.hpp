#ifndef MOCK_HEATER_H
#define MOCK_HEATER_H

#include "Heater.hpp"

class MockHeater : public Heater {
public:
    explicit MockHeater(int pin);

    void turnOn() override ;

    void turnOff() override ;

    void setPercentage(int percentage) override ;

    int getPercentage() override ;

    void update() override ;

private:
    int _pin;
};

#endif //MOCK_HEATER_H

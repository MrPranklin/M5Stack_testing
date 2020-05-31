#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

#include "ArtificialSource.hpp"

class LedController : public ArtificialSource {
public:
    explicit LedController(int pin);

    void turnOn() override;

    void turnOff() override;

    bool toggle() override;

    void setPercentage(int percentage) override;

    int getPercentage() override;

    void update() override;

protected:
    int _pin;
    int _currentPercentage{0};
};

#endif // LEDCONTROLLER_H
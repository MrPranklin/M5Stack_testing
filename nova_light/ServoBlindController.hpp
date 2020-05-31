#ifndef SERVOBLINDCONTROLLER_H
#define SERVOBLINDCONTROLLER_H

#include "NaturalSource.hpp"
#include <Servo.h>

class ServoBlindController : public NaturalSource {
public:
    ServoBlindController(int pin, int fullyOpenedPosition);

    void turnOn() override;

    void turnOff() override;

    int getPercentage() override;

    void setPercentage(int percentage) override;

    void update() override ;

private:
    int _pin;
    int _fullyOpenedPosition{180};
    Servo *_servo;
};

#endif // SERVOBLINDCONTROLLER_H
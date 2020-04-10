#ifndef SERVOBLINDCONTROLLER_H
#define SERVOBLINDCONTROLLER_H

#include "Blinds.hpp"
#include <Servo.h>

class ServoBlindController : public Blinds {
public:
    ServoBlindController(int pin, int fullyOpenedPosition);

    void open() override;

    void close() override;

    int getPercentage() override;

    void setPercentage(int percentage) override;

private:
    int _pin;
    int _fullyOpenedPosition{180};
    Servo *_servo;
};

#endif // SERVOBLINDCONTROLLER_H
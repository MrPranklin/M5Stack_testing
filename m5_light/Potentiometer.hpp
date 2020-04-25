#ifndef POTENTIOMENTER_H
#define POTENTIOMENTER_H

#include "BrightnessSensor.hpp"

class Potentiometer : public BrightnessSensor {
public:
    explicit Potentiometer(int pin);

    int readBrightness() override;

private:
    int _pin;
};

#endif //POTENTIOMENTER_H

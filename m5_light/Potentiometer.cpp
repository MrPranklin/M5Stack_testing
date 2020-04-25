#include <Arduino.h>
#include "Potentiometer.hpp"

Potentiometer::Potentiometer(int pin) {
    this->_pin = pin;
    pinMode(pin, INPUT);
}

int Potentiometer::readBrightness() {
    return map(analogRead(_pin), 0, 4095, 0, 100);
}


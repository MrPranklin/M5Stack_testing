#include <Arduino.h>
#include "Potentiometer.hpp"

Potentiometer::Potentiometer(int pin) {
    this->_pin = pin;
    pinMode(pin, INPUT);
}

int Potentiometer::readBrightness() {
    int value = analogRead(_pin);
    Serial.println(value);
    return value;
}


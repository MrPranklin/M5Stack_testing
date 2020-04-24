#include "FanController.hpp"
#include <Arduino.h>

FanController::FanController(int pin) {
    this->_pin = pin;
    pinMode(_pin, OUTPUT);
}

void FanController::turnOn() {
    this->_isEnabled = true;
    digitalWrite(this->_pin, HIGH);
}

void FanController::turnOff() {
    this->_isEnabled = false;
    digitalWrite(this->_pin, LOW);
}

bool FanController::isEnabled() {
    return this->_isEnabled;
}

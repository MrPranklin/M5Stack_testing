#include "FanController.hpp"
#include <Arduino.h>

FanController::FanController(int pin) {
    this->_pin = pin;
    pinMode(_pin, OUTPUT);
}

void FanController::turnOn() {
    int valueToWrite = map(getPercentage(), 0, 100, 0, 255);
    analogWrite(this->_pin, valueToWrite);
    this->_isEnabled = true;
}

void FanController::turnOff() {
    this->_isEnabled = false;
    analogWrite(this->_pin, 0);
}

void FanController::setPercentage(int percentage) {
    if (percentage < 0) {
        this->_currentPercentage = 0;
    } else if (percentage > 100) {
        this->_currentPercentage = 100;
    } else {
        this->_currentPercentage = percentage;
    }

    turnOn();
}

int FanController::getPercentage() {
    return this->_currentPercentage;
}

void FanController::update() {

}

#include "LedController.hpp"
#include "Arduino.h"

LedController::LedController(int pin) {
    this->_pin = pin;
    pinMode(pin, OUTPUT);
}

void LedController::turnOn() {
    int valueToWrite = map(getPercentage(), 0, 100, 0, 255);
    analogWrite(this->_pin, valueToWrite);
    this->_isOn = true;
}

void LedController::turnOff() {
    analogWrite(this->_pin, 0);
    this->_isOn = false;
}

bool LedController::toggle() {
    if(isOn()){
        turnOff();
    } else {
        turnOn();
    }
    return isOn();
}

void LedController::setPercentage(int percentage) {
    if (percentage < 0) {
        this->_currentPercentage = 0;
    } else if (percentage > 100) {
        this->_currentPercentage = 100;
    } else {
        this->_currentPercentage = percentage;
    }

    turnOn();
}

int LedController::getPercentage() {
    return this->_currentPercentage;
}

bool LedController::isOn() {
    return this->_isOn;
}
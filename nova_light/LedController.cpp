#include "LedController.hpp"
#include "Arduino.h"

LedController::LedController(int pin) {
    this->_pin = pin;
    pinMode(pin, OUTPUT);
}

void LedController::turnOn() {
    int valueToWrite = map(getPercentage(), 0, 100, 0, 255);
    analogWrite(this->_pin, valueToWrite);
    this->_isEnabled = true;
}

void LedController::turnOff() {
    analogWrite(this->_pin, 0);
    this->_isEnabled = false;
}

bool LedController::toggle() {
    if(_isEnabled){
        turnOff();
    } else {
        turnOn();
    }
    return _isEnabled;
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

void LedController::update() {

}

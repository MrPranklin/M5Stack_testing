#include "MockHeater.hpp"
#include <Arduino.h>

MockHeater::MockHeater(int pin) : Heater() {
    this->_pin = pin;
    pinMode(this->_pin, OUTPUT);
}

void MockHeater::turnOn() {
    int valueToWrite = map(getPercentage(), 0, 100, 0, 255);
    analogWrite(this->_pin, valueToWrite);
    this->_isEnabled = true;
}

void MockHeater::turnOff() {
    this->_isEnabled = false;
    analogWrite(this->_pin, 0);
}

bool MockHeater::isEnabled(){
  return _isEnabled;
}

void MockHeater::setPercentage(int percentage) {
    if (percentage < 0) {
        this->_currentPercentage = 0;
    } else if (percentage > 100) {
        this->_currentPercentage = 100;
    } else {
        this->_currentPercentage = percentage;
    }

    turnOn();
}

int MockHeater::getPercentage() {
    return this->_currentPercentage;
}

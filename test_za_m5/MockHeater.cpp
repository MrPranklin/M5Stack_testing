#include "MockHeater.hpp"
//#include <Arduino.h>

MockHeater::MockHeater(int pin) : Heater() {
    this->_pin = pin;
//    pinMode(this->_pin, OUTPUT);
}

void MockHeater::turnOn() {
    this->_isEnabled = true;
//    digitalWrite(this->_pin, HIGH);
}

void MockHeater::turnOff() {
    this->_isEnabled = false;
//    digitalWrite(this->_pin, LOW);
}

bool MockHeater::isEnabled(){
  return _isEnabled;
}
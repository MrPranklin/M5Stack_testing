#include "ServoBlindController.hpp"

ServoBlindController::ServoBlindController(int pin, int fullyOpenedPosition) {
    this->_pin = pin;
    this->_fullyOpenedPosition = fullyOpenedPosition;
    this->_servo = new Servo();
    _servo->attach(pin);
}

void ServoBlindController::turnOn() {
    setPercentage(100);
}

void ServoBlindController::turnOff() {
    setPercentage(0);
}

int ServoBlindController::getPercentage() {
    return this->_currentPercentage;
}

void ServoBlindController::setPercentage(int percentage) {
    if (percentage < 0) {
        this->_currentPercentage = 0;
    } else if (percentage > 100) {
        this->_currentPercentage = 100;
    } else {
        this->_currentPercentage = percentage;
    }

    int positionToWrite = (getPercentage() / 100.0) * this->_fullyOpenedPosition;
    _servo->write(positionToWrite);
}

void ServoBlindController::update() {

}

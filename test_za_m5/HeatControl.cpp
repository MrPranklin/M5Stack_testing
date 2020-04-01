#include "HeatControl.hpp"
#include "DHT22_C.hpp"
#include "mqtt.hpp"

HeatControl::HeatControl(TempSensor *tempSensor, PubSubClient client) {
    this->_tempSensor = tempSensor;
    this->_client = client;
    this->_isEnabled = false;
    this->_isHeatingOn = false;
    this->_isCoolingOn = false;
    this->_targetTemp = 0;
    this->_currentTemp = 0;
}

void HeatControl::turnEverythingOff() {
    turnOffHeating();
    turnOffCooling();
}

float HeatControl::getCurrentTemp() {
    return _tempSensor->readTemperature();
}

void HeatControl::setTargetTemp(float temp) {
    this->_targetTemp = temp;
}

float HeatControl::getTargetTemp() {
    return this->_targetTemp;
}

float HeatControl::incrementTargetTemp(float amount) {
    setTargetTemp(getTargetTemp() + amount);
    return getTargetTemp();
}


void HeatControl::turnOnCooling() {
    turnOffHeating();
    if (!getCoolingStatus()) {
        Serial.println("HeatControl: cooling ON");
        setCoolingStatus(true);
        mqtt::sendTurnOnCooling(this->_client);
    }
}

void HeatControl::turnOffCooling() {
    if (getCoolingStatus()) {
        Serial.println("HeatControl: cooling OFF");
        setCoolingStatus(false);
        mqtt::sendTurnOffCooling(this->_client);
    }
}

bool HeatControl::getCoolingStatus() {
    return this->_isCoolingOn;
}

void HeatControl::setCoolingStatus(bool isCoolingOn) {
    this->_isCoolingOn = isCoolingOn;
}

void HeatControl::turnOnHeating() {
    turnOffCooling();
    if (!getHeatingStatus()) {
        Serial.println("HeatControl: heating ON");
        setHeatingStatus(true);
        mqtt::sendTurnOnHeating(this->_client);
    }
}

void HeatControl::turnOffHeating() {
    if (getHeatingStatus()) {
        Serial.println("HeatControl: heating OFF");
        setHeatingStatus(false);
        mqtt::sendTurnOffHeating(this->_client);
    }
}

bool HeatControl::getHeatingStatus() {
    return this->_isHeatingOn;
}

void HeatControl::setHeatingStatus(bool isHeatingOn) {
    this->_isHeatingOn = isHeatingOn;
}

void HeatControl::enable() {
    this->_isEnabled = true;
}

void HeatControl::disable() {
    this->_isEnabled = false;
}

bool HeatControl::isEnabled() {
    return _isEnabled;
}

bool HeatControl::update() {
    bool isEnabled_ = isEnabled();

    if (isEnabled_) {
        _currentTemp = getCurrentTemp();

        if (_currentTemp - _targetTemp >= 1) {
            turnOnCooling();
        } else if (_currentTemp - _targetTemp <= -1) {
            turnOnHeating();
        } else {
            turnEverythingOff();
        }
    }
    return isEnabled_;
}
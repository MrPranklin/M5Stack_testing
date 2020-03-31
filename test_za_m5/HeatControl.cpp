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
    this->_targetTemp += amount;
    return this->_targetTemp;
}


void HeatControl::turnOnCooling() {
    turnOffHeating();
    if (!this->_isCoolingOn) {
        Serial.println("HeatControl: cooling ON");
        this->_isCoolingOn = true;
        mqtt::sendTurnOnCooling(this->_client);
    }
}

void HeatControl::turnOffCooling() {
    if (this->_isCoolingOn) {
        Serial.println("HeatControl: cooling OFF");
        this->_isCoolingOn = false;
        mqtt::sendTurnOffCooling(this->_client);
    }
}

void HeatControl::turnOnHeating() {
    turnOffCooling();
    if (!this->_isHeatingOn) {
        Serial.println("HeatControl: heating ON");
        this->_isHeatingOn = true;
        mqtt::sendTurnOnHeating(this->_client);
    }
}

void HeatControl::turnOffHeating() {
    if (this->_isHeatingOn) {
        Serial.println("HeatControl: heating OFF");
        this->_isHeatingOn = false;
        mqtt::sendTurnOffHeating(this->_client);
    }
}

void HeatControl::enable() {
    this->_isEnabled = true;
}

void HeatControl::disable() {
    this->_isEnabled = false;
    turnEverythingOff();
}

bool HeatControl::isEnabled() {
    return _isEnabled;
}

bool HeatControl::update() {
    bool isEnabled_ = isEnabled();
    _currentTemp = getCurrentTemp();

    if (isEnabled_) {
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
#include <cmath>
#include <Arduino.h>

#include "HeatControl.hpp"
#include "DHT22_C.hpp"
#include "mqtt.hpp"

HeatControl::HeatControl(TempSensor *tempSensor, PubSubClient client) {
    this->_tempSensor = tempSensor;
    this->_client = client;
}

HeatControl::~HeatControl() {
    delete _tempSensor;
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
    bool valuesChanged = false;

    if (isEnabled()) {
        _currentTemp = getCurrentTemp();

        double tempDiff = _currentTemp - _targetTemp; // positive if cooling is required

        double absTempDiff = std::abs(tempDiff);;
        int valueToWrite = std::round(absTempDiff * 20);
        if (valueToWrite > 100) {
            valueToWrite = 100;
        }

        if (tempDiff > 0) {
            valuesChanged = valuesChanged || setCoolingPercentage(valueToWrite);
            valuesChanged = valuesChanged || setHeatingPercentage(0);
        } else if (tempDiff < 0) {
            valuesChanged = valuesChanged || setHeatingPercentage(valueToWrite);
            valuesChanged = valuesChanged || setCoolingPercentage(0);
        } else {
            valuesChanged = valuesChanged || setHeatingPercentage(0);
            valuesChanged = valuesChanged || setCoolingPercentage(0);
        }
    }

    return valuesChanged;
}

int HeatControl::getCoolingPercentage() {
    return _currentCoolingPercentage;
}

int HeatControl::getHeatingPercentage() {
    return _currentHeatingPercentage;
}

bool HeatControl::setCoolingPercentage(int percentage) {
    // if it's being set to 0, but wasn't 0 before, update
    if (percentage == 0 && _currentCoolingPercentage != 0) {
        _currentCoolingPercentage = percentage;
        return true;
    }

    // if the diff is more than 5%, update
    if ((std::abs(percentage - _currentCoolingPercentage) >= 5)) {
        _currentCoolingPercentage = percentage;
        return true;
    }

    return false;
}

bool HeatControl::setHeatingPercentage(int percentage) {
// if it's being set to 0, but wasn't 0 before, update
    if (percentage == 0 && _currentHeatingPercentage != 0) {
        _currentHeatingPercentage = percentage;
        return true;
    }

    // if the diff is more than 5%, update
    if ((std::abs(percentage - _currentHeatingPercentage) >= 5)) {
        _currentHeatingPercentage = percentage;
        return true;
    }

    return false;
}

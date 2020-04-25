#include <cmath>
#include <Arduino.h>

#include "LightControl.hpp"
#include "mqtt.hpp"

LightControl::LightControl(BrightnessSensor *lightSensor, PubSubClient client) {
    this->_brightnessSensor = lightSensor;
    this->_client = client;
}

LightControl::~LightControl() {
    delete _brightnessSensor;
}

int LightControl::getCurrentBrightness() {
    return _brightnessSensor->readBrightness();
}

void LightControl::setTargetBrightness(int brightness) {
    this->_targetBrightness = brightness;
}

int LightControl::getTargetBrightness() {
    return this->_targetBrightness;
}

int LightControl::incrementTargetBrightness(int amount) {
    setTargetBrightness(getTargetBrightness() + amount);
    return getTargetBrightness();
}

void LightControl::enable() {
    this->_isEnabled = true;
}

void LightControl::disable() {
    this->_isEnabled = false;
}

bool LightControl::isEnabled() {
    return _isEnabled;
}

bool LightControl::update() {
    bool valuesChanged = false;

    bool isEnabled = this->isEnabled();

    if (isEnabled) {
        _currentBrightness = getCurrentBrightness();

        double tempDiff = _currentBrightness - _targetBrightness; // positive if darkening is required

        double absTempDiff = std::abs(tempDiff);;
        int valueToWrite = std::round(absTempDiff * 20);
        if (valueToWrite > 100) {
            valueToWrite = 100;
        }

        if (tempDiff > 0) {
            valuesChanged = valuesChanged || setArtificialLightPercentage(valueToWrite);
            valuesChanged = valuesChanged || setNaturalLightPercentage(0);
        } else if (tempDiff < 0) {
            valuesChanged = valuesChanged || setNaturalLightPercentage(valueToWrite);
            valuesChanged = valuesChanged || setArtificialLightPercentage(0);
        } else {
            valuesChanged = valuesChanged || setNaturalLightPercentage(0);
            valuesChanged = valuesChanged || setArtificialLightPercentage(0);
        }
    }

    return isEnabled && valuesChanged;
}

int LightControl::getArtificialLightPercentage() {
    return _currentNaturalLightPercentage;
}

int LightControl::getNaturalLightPercentage() {
    return _currentArtificialLightPercentage;
}

bool LightControl::setArtificialLightPercentage(int percentage) {
    // if it's being set to 0, but wasn't 0 before, update
    if (percentage == 0 && _currentNaturalLightPercentage != 0) {
        _currentNaturalLightPercentage = percentage;
        return true;
    }

    // if the diff is more than 5%, update
    if ((std::abs(percentage - _currentNaturalLightPercentage) >= 5)) {
        _currentNaturalLightPercentage = percentage;
        return true;
    }

    return false;
}

bool LightControl::setNaturalLightPercentage(int percentage) {
// if it's being set to 0, but wasn't 0 before, update
    if (percentage == 0 && _currentArtificialLightPercentage != 0) {
        _currentArtificialLightPercentage = percentage;
        return true;
    }

    // if the diff is more than 5%, update
    if ((std::abs(percentage - _currentArtificialLightPercentage) >= 5)) {
        _currentArtificialLightPercentage = percentage;
        return true;
    }

    return false;
}

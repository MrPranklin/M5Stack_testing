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

int LightControl::getBrightness() {
    return _brightnessSensor->readBrightness();
}

void LightControl::setTargetBrightness(int brightness) {
    if (brightness < 0) {
        this->_targetBrightness = 0;
    } else if (brightness > 100) {
        this->_targetBrightness = 100;
    } else {
        this->_targetBrightness = brightness;
    }
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

    bool isEnabled = _isEnabled;
    _currentBrightness = getBrightness();

    if (isEnabled) {

        int diff = _currentBrightness - _targetBrightness; // positive if we need less light
        int absDiff = std::abs(diff);

        if (getIsSunUp()) {
            if (diff < -5) { // make it darker
                setArtificialLightPercentage(0);
                setNaturalLightPercentage(getNaturalLightPercentage() - 5);
            } else if (diff > 5) { // make it brighter
                if (!setNaturalLightPercentage(getNaturalLightPercentage() + 5)) {
                    // if increasing natural light fails, increase artificial
                    setArtificialLightPercentage(getArtificialLightPercentage() + 5);
                }
            }
        } else {
            setNaturalLightPercentage(0);
            if (diff < -5) { // make it darker
                setArtificialLightPercentage(getArtificialLightPercentage() - 5);
            } else if (diff > 5) { // make it brighter
                setArtificialLightPercentage(getArtificialLightPercentage() + 5);
            }
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
    // if being set to >= 100 and it isn't 100 yet, update
    if (percentage >= 100 && _currentArtificialLightPercentage < 100) {
        _currentArtificialLightPercentage = 100;
        return true;
    } else if (percentage >= 100 && _currentArtificialLightPercentage == 100) {
        return false;
    }

    if (percentage <= 0 && _currentArtificialLightPercentage > 0) {
        _currentArtificialLightPercentage = 0;
        return true;
    } else if (percentage <= 0 && _currentArtificialLightPercentage == 0) {
        return false;
    }

    // if the diff is more than 5%, update
    if ((std::abs(percentage - _currentArtificialLightPercentage) >= 5)) {
        _currentArtificialLightPercentage = percentage;
        return true;
    }

    return false;
}

bool LightControl::setNaturalLightPercentage(int percentage) {
    if (percentage >= 100 && _currentNaturalLightPercentage < 100) {
        _currentNaturalLightPercentage = 100;
        return true;
    } else if (percentage >= 100 && _currentNaturalLightPercentage == 100) {
        return false;
    }

    if (percentage <= 0 && _currentNaturalLightPercentage > 0) {
        _currentNaturalLightPercentage = 0;
        return true;
    } else if (percentage <= 0 && _currentNaturalLightPercentage == 0) {
        return false;
    }

    // if the diff is more than 5%, update
    if ((std::abs(percentage - _currentNaturalLightPercentage) >= 5)) {
        _currentNaturalLightPercentage = percentage;
        return true;
    }

    return false;
}

bool LightControl::getIsSunUp() {
    return this->_isSunUp;
}

void LightControl::setIsSunUp(bool isSunUp) {
    this->_isSunUp = isSunUp;
}

#include <cmath>
#include <Arduino.h>

#include "LightControl.hpp"
#include "mqtt.hpp"

void LightControl::enable() {
    this->_isEnabled = true;
}

void LightControl::disable() {
    this->_isEnabled = false;
}

bool LightControl::isEnabled() {
    return _isEnabled;
}

int LightControl::getArtificialLightPercentage() {
    return _currentArtificialLightPercentage;
}

int LightControl::getNaturalLightPercentage() {
    return _currentNaturalLightPercentage;
}

bool LightControl::setArtificialLightPercentage(int percentage) {
    if (percentage > 100) {
        _currentArtificialLightPercentage = 100;
        return false;
    } else if (percentage < 0) {
        _currentArtificialLightPercentage = 0;
        return false;
    } else {
        _currentArtificialLightPercentage = percentage;
        return true;
    }
}

bool LightControl::setNaturalLightPercentage(int percentage) {
    if (percentage > 100) {
        _currentNaturalLightPercentage = 100;
        return false;
    } else if (percentage < 0) {
        _currentNaturalLightPercentage = 0;
        return false;
    } else {
        _currentNaturalLightPercentage = percentage;
        return true;
    }
}

int LightControl::incrementArtificialLight(int amount) {
    setArtificialLightPercentage(getArtificialLightPercentage() + amount);
    return getArtificialLightPercentage();
}

int LightControl::incrementNaturalLight(int amount) {
    setNaturalLightPercentage(getNaturalLightPercentage() + amount);
    return getNaturalLightPercentage();
}
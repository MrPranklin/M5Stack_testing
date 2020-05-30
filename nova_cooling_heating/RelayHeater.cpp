#include <Arduino.h>

#include "RelayHeater.hpp"

RelayHeater::RelayHeater(int pin, long dutyCyclePeriod) {
    _pin = pin;
    pinMode(pin, OUTPUT);
    _dutyCyclePeriod = dutyCyclePeriod;
}

void RelayHeater::turnOn() {
    _isEnabled = true;
}

void RelayHeater::turnOff() {
    _isEnabled = false;
}

void RelayHeater::setPercentage(int percentage) {
    int perc = 0;
    if (percentage > 100) {
        perc = 100;
    } else if (percentage < 0) {
        perc = 0;
    } else {
        perc = percentage;
    }

    _currentPercentage = perc;

    calcOnOffTime();
}

void RelayHeater::pwmOn() {
    digitalWrite(_pin, HIGH);
    _pwmState = true;
    Serial.println("RelayHeater: pwmOn");
}

void RelayHeater::pwmOff() {
    digitalWrite(_pin, LOW);
    _pwmState = false;
    Serial.println("RelayHeater: pwmOff");
}

void RelayHeater::calcOnOffTime() {

    _onTime = _dutyCyclePeriod * _currentPercentage / 100;
    _offTime = _dutyCyclePeriod - _onTime;
    Serial.print("RelayHeater: onTime: ");
    Serial.println(_onTime);
    Serial.print("RelayHeater: offTime: ");
    Serial.println(_offTime);
}

void RelayHeater::update() {
    if (_isEnabled) {
        if (_pwmState) {
            if (millis() - _lastPwmMillis > _onTime) {
                if (_offTime > 0) {
                    pwmOff();
                }
                _lastPwmMillis = millis();
            }
        } else {
            if (millis() - _lastPwmMillis > _offTime) {
                if (_onTime > 0) {
                    pwmOn();
                }
                _lastPwmMillis = millis();
            }
        }
    }
}

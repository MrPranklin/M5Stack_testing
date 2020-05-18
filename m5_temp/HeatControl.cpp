#include <cmath>
#include <Arduino.h>
#include <AutoPID.h>

#include "HeatControl.hpp"
#include "DHT22_C.hpp"
#include "mqtt.hpp"

#define KP 5
#define KI 0.1
#define OUTPUT_MIN -100
#define OUTPUT_MAX 100
#define PID_INTERVAL 1000
#define PID_THRESHOLD 5

HeatControl::HeatControl(TempSensor *tempSensor, PubSubClient client) {
    this->_tempSensor = tempSensor;
    this->_client = client;
    this->_autopid = new AutoPID(&_currentTempDouble, &_targetTempDouble, &PID_output, OUTPUT_MIN, OUTPUT_MAX, KP, KI,
                                 0);
    this->_autopid->setBangBang(PID_THRESHOLD); // if the diff is 5, rank it up to max
    this->_autopid->setTimeStep(PID_INTERVAL); // recalculate every n ms
}

HeatControl::~HeatControl() {
    delete _tempSensor;
}

float HeatControl::getCurrentTemp() {
    return _tempSensor->readTemperature();
}

void HeatControl::setTargetTemp(float temp) {
    _autopid->reset();
    this->_targetTemp = temp;
    _targetTempDouble = static_cast<double>(temp);
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

    bool isEnabled = this->isEnabled();

    if (this->isEnabled()) {
        _currentTemp = getCurrentTemp();
        _currentTempDouble = static_cast<double>(_currentTemp);
        _autopid->run();


        if (PID_output != PID_output_old) {
            PID_output_old = PID_output;

            Serial.println(PID_output);

            if (PID_output < 0) {
                setCoolingPercentage(std::abs(PID_output));
                setHeatingPercentage(0);
            } else {
                setHeatingPercentage(PID_output);
                setCoolingPercentage(0);
            }
        }
    }

    return isEnabled && valuesChanged;
}

int HeatControl::getCoolingPercentage() {
    return _currentCoolingPercentage;
}

int HeatControl::getHeatingPercentage() {
    return _currentHeatingPercentage;
}

void HeatControl::setCoolingPercentage(int percentage) {
    _currentCoolingPercentage = percentage;
}

void HeatControl::setHeatingPercentage(int percentage) {
    _currentHeatingPercentage = percentage;
}

#include "HeatControl.hpp"

HeatControl::HeatControl(TempSensor *tempSensor, std::vector<Cooler *> coolers, std::vector<Heater *> heaters) {
    this->tempSensor = tempSensor;
    this->coolers = coolers;
    this->heaters = heaters;
    this->isEnabled = false;
    this->targetTemp = 0;
    this->currentTemp = 0;
}

HeatControl::~HeatControl() {
    delete tempSensor;
    tempSensor = nullptr;

    delete coolers;
    coolers = nullptr;

    delete heaters;
    heaters = nullptr;
}

void HeatControl::turnEverythingOff() {
    disable()

    turnOffAllHeaters();
    turnOffAllCoolers();
}

float HeatControl::getCurrentTemp(TempSensor *tempSensor) {
    return tempSensor->readTemperature();
}

void HeatControl::setTargetTemp(double temp) {
    this->targetTemp = temp;
}

void HeatControl::turnOnCooler(Cooler *c) {
    c->turnOn();
}

void HeatControl::turnOffCooler(Cooler *c) {
    c->turnOff();
}

void HeatControl::turnOnAllCoolers() {
    for (int i = 0; i < heaters.size(); i++) {
        turnONCooler(coolers[i]);
    }
}

void HeatControl::turnOffAllCoolers() {
    for (int i = 0; i < heaters.size(); i++) {
        turnOffCooler(coolers[i]);
    }
}

void HeatControl::turnOnHeater(Heater *h) {
    h->turnOn();
}

void HeatControl::turnOffHeater(Heater *h) {
    h->turnOff();
}

void HeatControl::turnOnAllHeaters() {
    for (int i = 0; i < heaters.size(); i++) {
        turnOnHeater(heaters[i]);
    }
}

void HeatControl::turnOffAllHeaters() {
    for (int i = 0; i < heaters.size(); i++) {
        turnOffHeater(heaters[i]);
    }
}

void HeatControl::enable() {
    this->isEnabled = true;
}

void HeatControl::disable() {
    this->isEnabled = false;
}

bool HeatControl::isEnabled() {
    return isEnabled;
}

bool HeatControl::update() {
    if (isEnabled()) {

        currentTemp = getCurrentTemp(tempSensor);

        if (currentTemp > targetTemp) {

        } else if (currentTemp < targetTemp) {

        } else {
            turnEverythingOff();
        }
    }
    return isEnabled();
}

void HeatControl::setTargetTemp(float temp) {
    this->targetTemp = temp;
}
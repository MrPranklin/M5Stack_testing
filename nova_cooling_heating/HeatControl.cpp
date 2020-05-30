#include "HeatControl.hpp"

#include <utility>

HeatControl::HeatControl(std::vector<Cooler *> coolers, std::vector<Heater *> heaters) {
    this->_coolers = std::move(coolers);
    this->_heaters = std::move(heaters);
}

HeatControl::~HeatControl() = default;

void HeatControl::turnEverythingOff() {
    turnOffAllHeaters();
    turnOffAllCoolers();
}

void HeatControl::turnOnCooler(Cooler *c) {
    c->turnOn();
}

void HeatControl::turnOffCooler(Cooler *c) {
    c->turnOff();
}

void HeatControl::turnOnAllCoolers() {
    for (unsigned int i = 0; i < _coolers.size(); i++) {
        turnOnCooler(this->_coolers.at(i));
    }
}

void HeatControl::turnOffAllCoolers() {
    for (int i = 0; i < _coolers.size(); i++) {
        turnOffCooler(this->_coolers.at(i));
    }
}

void HeatControl::turnOnHeater(Heater *h) {
    h->turnOn();
}

void HeatControl::turnOffHeater(Heater *h) {
    h->turnOff();
}

void HeatControl::turnOnAllHeaters() {
    for (int i = 0; i < _heaters.size(); i++) {
        turnOnHeater(this->_heaters.at(i));
    }
}

void HeatControl::turnOffAllHeaters() {
    for (int i = 0; i < _heaters.size(); i++) {
        turnOffHeater(this->_heaters.at(i));
    }
}

void HeatControl::setPercentageToAllHeaters(int percentage) {
    for (int i = 0; i < _heaters.size(); i++) {
        setPercentageToHeater(this->_heaters.at(i), percentage);
    }
}

void HeatControl::setPercentageToAllCoolers(int percentage) {
    for (int i = 0; i < _coolers.size(); i++) {
        setPercentageToCooler(this->_coolers.at(i), percentage);
    }
}

void HeatControl::setPercentageToCooler(Cooler *c, int percentage) {
    c->setPercentage(percentage);
}

void HeatControl::setPercentageToHeater(Heater *h, int percentage) {
    h->setPercentage(percentage);
}

void HeatControl::update() {
    for (auto & heater : _heaters) {
        heater->update();
    }
    for(auto & cooler : _coolers){
        cooler->update();
    }
}

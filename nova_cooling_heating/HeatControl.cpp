#include "HeatControl.hpp"

HeatControl::HeatControl(std::vector<Cooler *> *coolers, std::vector<Heater *> *heaters){
  this->_coolers = coolers;
  this->_heaters = heaters;
  this->_isEnabled = false;
}

HeatControl::~HeatControl() {
  delete _coolers;
  _coolers = nullptr;

  delete _heaters;
  _heaters = nullptr;
}

void HeatControl::turnEverythingOff() {
  disable();

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
  for (unsigned int i = 0; i < _coolers->size(); i++) {
    turnOnCooler(this->_coolers->at(i));
  }
}

void HeatControl::turnOffAllCoolers() {
  for (int i = 0; i < _coolers->size(); i++) {
    turnOffCooler(this->_coolers->at(i));
  }
}

void HeatControl::turnOnHeater(Heater *h) {
  h->turnOn();
}

void HeatControl::turnOffHeater(Heater *h) {
  h->turnOff();
}

void HeatControl::turnOnAllHeaters() {
  for (int i = 0; i < _heaters->size(); i++) {
    turnOnHeater(this->_heaters->at(i));
  }
}

void HeatControl::turnOffAllHeaters() {
  for (int i = 0; i < _heaters->size(); i++) {
    turnOffHeater(this->_heaters->at(i));
  }
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
#include "LightControl.hpp"
#include "ArtificialSource.hpp"
#include "NaturalSource.hpp"

#include <utility>

LightControl::LightControl(std::vector<ArtificialSource *> artificials, std::vector<NaturalSource *> naturals) {
    this->_artificials = std::move(artificials);
    this->_naturals = std::move(naturals);
}

LightControl::~LightControl() = default;

void LightControl::turnEverythingOff() {
    turnOffAllNaturals();
    turnOffAllArtificials();
}

void LightControl::turnOnArtificial(ArtificialSource *c) {
    c->turnOn();
}

void LightControl::turnOffArtificial(ArtificialSource *c) {
    c->turnOff();
}

void LightControl::turnOnAllArtificials() {
    for (auto &a: _artificials) {
        turnOnArtificial(a);
    }
}

void LightControl::turnOffAllArtificials() {
    for (auto &a: _artificials) {
        turnOffArtificial(a);
    }
}

void LightControl::turnOnNatural(NaturalSource *n) {
    n->turnOn();
}

void LightControl::turnOffNatural(NaturalSource *n) {
    n->turnOff();
}

void LightControl::turnOnAllNaturals() {
    for (auto &n : _naturals) {
        turnOnNatural(n);
    }
}

void LightControl::turnOffAllNaturals() {
    for (auto &n : _naturals) {
        turnOffNatural(n);
    }
}

void LightControl::setPercentageToAllNaturals(int percentage) {
    for (auto &n : _naturals) {
        setPercentageToNatural(n, percentage);
    }
}

void LightControl::setPercentageToAllArtificials(int percentage) {
    for (auto &a : _artificials) {
        setPercentageToArtificial(a, percentage);
    }
}

void LightControl::setPercentageToArtificial(ArtificialSource *a, int percentage) {
    a->setPercentage(percentage);
}

void LightControl::setPercentageToNatural(NaturalSource *n, int percentage) {
    n->setPercentage(percentage);
}

void LightControl::update() {
    for (auto &n : _naturals) {
        n->update();
    }
    for (auto &a : _artificials) {
        a->update();
    }
}

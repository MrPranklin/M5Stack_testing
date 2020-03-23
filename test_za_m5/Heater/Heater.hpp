#ifndef HEATER_H
#define HEATER_H

class Heater {
public:
    Heater();

    virtual void turnOn() = 0;

    virtual void turnOff() = 0;

private:
    bool isEnabled;
};

#endif //HEATER_H

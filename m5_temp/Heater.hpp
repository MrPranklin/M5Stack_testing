#ifndef HEATER_H
#define HEATER_H

class Heater {
public:
    virtual void turnOn() = 0;

    virtual void turnOff() = 0;

    virtual bool isEnabled() = 0;

protected:
    bool _isEnabled{false};
};

#endif //HEATER_H
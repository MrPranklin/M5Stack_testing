#ifndef HEATER_H
#define HEATER_H

class Heater {
public:
    virtual void turnOn() = 0;

    virtual void turnOff() = 0;

    virtual bool isEnabled() = 0;

    virtual void setPercentage(int percentage) = 0;

    virtual int getPercentage() = 0;

protected:
    bool _isEnabled{false};
    int _currentPercentage{0};
};

#endif //HEATER_H
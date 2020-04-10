#ifndef LIGHT_H
#define LIGHT_H

class Light {
public:
    virtual void turnOn() = 0;

    virtual void turnOff() = 0;

    virtual bool isOn() = 0;

    virtual bool toggle() = 0;

    virtual void setPercentage(int percentage) = 0;

    virtual int getPercentage() = 0;

protected:
    bool _isOn{false};
    int _currentPercentage{0};
};

#endif // LIGHT_H
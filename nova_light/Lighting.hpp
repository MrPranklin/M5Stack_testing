#ifndef LIGHTING_H
#define LIGHTING_H

class Lighting{
public:
    virtual void turnOn() = 0;

    virtual void turnOff() = 0;

    virtual bool isEnabled() = 0;

protected:
    bool _isEnabled{false};
};

#endif // LIGHTING_H
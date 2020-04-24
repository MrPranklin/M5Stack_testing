#ifndef COOLER_H
#define COOLER_H

class Cooler {
public:
    virtual void turnOn() = 0;

    virtual void turnOff() = 0;

    virtual bool isEnabled() = 0;

protected:
    bool _isEnabled{false};
};

#endif //COOLER_H

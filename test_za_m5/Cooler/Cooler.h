#ifndef COOLER_H
#define COOLER_H

class Cooler {
public:
    Cooler();

    virtual void turnOn() = 0;

    virtual void turnOff() = 0;

private:
    bool isEnabled();
};

#endif //COOLER_H

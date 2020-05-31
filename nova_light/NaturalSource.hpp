#ifndef NATURALSOURCE_H
#define NATURALSOURCE_H

class NaturalSource {
public:
    virtual void turnOn() = 0;

    virtual void turnOff() = 0;

    virtual void setPercentage(int percentage) = 0;

    virtual int getPercentage() = 0;

    virtual void update() = 0;

protected:
    bool _isEnabled{false};
    int _currentPercentage{0};
};

#endif // NATURALSOURCE_H
#ifndef ARTIFICIALSOURCE_H
#define ARTIFICIALSOURCE_H

class ArtificialSource {
public:
    virtual void turnOn() = 0;

    virtual void turnOff() = 0;

    virtual bool toggle() = 0;

    virtual void setPercentage(int percentage) = 0;

    virtual int getPercentage() = 0;

    virtual void update() = 0;

protected:
    bool _isEnabled{false};
    int _currentPercentage{0};
};

#endif // ARTIFICIALSOURCE_H
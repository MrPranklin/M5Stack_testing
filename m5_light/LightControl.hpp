#ifndef LIGHT_CONTROL_H
#define LIGHT_CONTROL_H

#include <vector>

class LightControl {
public:
    LightControl() = default;

    ~LightControl() = default;

    virtual void enable();

    virtual void disable();

    virtual bool isEnabled();

    virtual int getArtificialLightPercentage();

    virtual int getNaturalLightPercentage();

    virtual bool setArtificialLightPercentage(int percentage);

    virtual bool setNaturalLightPercentage(int percentage);

    virtual int incrementArtificialLight(int amount);

    virtual int incrementNaturalLight(int amount);

protected:
    bool _isEnabled{false};

    int _currentNaturalLightPercentage{0};
    int _currentArtificialLightPercentage{0};
};

#endif //LIGHT_CONTROL_H

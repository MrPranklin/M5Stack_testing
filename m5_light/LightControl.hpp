#ifndef LIGHT_CONTROL_H
#define LIGHT_CONTROL_H

#include <PubSubClient.h>

#include "BrightnessSensor.hpp"
#include <vector>

class LightControl {
public:
    LightControl(BrightnessSensor *lightSensor, PubSubClient client);

    ~LightControl();

    virtual void setTargetBrightness(int brightness);

    virtual bool update();

    virtual void enable();

    virtual void disable();

    virtual bool isEnabled();

    virtual int getTargetBrightness();

    virtual int incrementTargetBrightness(int amount);

    virtual int getArtificialLightPercentage();

    virtual int getNaturalLightPercentage();

    virtual bool setArtificialLightPercentage(int percentage);

    virtual bool setNaturalLightPercentage(int percentage);

    virtual void setIsSunUp(bool isSunUp);

    virtual bool getIsSunUp();

protected:
    virtual int getBrightness();

    BrightnessSensor *_brightnessSensor;
    PubSubClient _client;
    bool _isEnabled{false};
    int _targetBrightness{0};
    int _currentBrightness{0};

    int _currentNaturalLightPercentage{0};
    int _currentArtificialLightPercentage{0};
    bool _isSunUp{true};
};

#endif //LIGHT_CONTROL_H

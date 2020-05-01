#ifndef LIGHT_CONTROL_H
#define LIGHT_CONTROL_H

#include <PubSubClient.h>

#include "BrightnessSensor.hpp"
#include <vector>

class LightControl {
public:
    explicit LightControl(PubSubClient client);

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
    PubSubClient _client;
    bool _isEnabled{false};

    int _currentNaturalLightPercentage{0};
    int _currentArtificialLightPercentage{0};
};

#endif //LIGHT_CONTROL_H

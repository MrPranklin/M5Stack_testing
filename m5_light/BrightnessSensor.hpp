#ifndef BRIGHTNESS_SENSOR_H
#define BRIGHTNESS_SENSOR_H

class BrightnessSensor {
public:
    virtual ~BrightnessSensor() = default;

    virtual int readBrightness() = 0;
};

#endif //BRIGHTNESS_SENSOR_H

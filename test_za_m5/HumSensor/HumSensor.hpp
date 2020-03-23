#ifndef HUM_SENSOR_H
#define HUM_SENSOR_H

class HumSensor {
public:
    virtual float readHumidity() = 0;
};

#endif //HUM_SENSOR_H

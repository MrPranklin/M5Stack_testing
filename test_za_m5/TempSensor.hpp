#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

class TempSensor {
public:
    virtual float readTemperature() = 0;
};

#endif //TEMP_SENSOR_H

#include "BME280.hpp"
#include <Wire.h>
#include <Arduino.h>

#define SENSOR_REST_TIME 1000

unsigned BME280::begin() {
    unsigned status;

    // default settings
    status = bme.begin(0x76, &Wire);
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x");
        Serial.println(bme.sensorID(), 16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        delay(10);
    }
    return status;
}

float BME280::readTemperature() {
    if (isSensorReady(SENSOR_REST_TIME)) {
        lastTemp = bme.readTemperature();
        lastHum = bme.readHumidity();
    }
    return lastTemp;
}

float BME280::readHumidity() {
    if (isSensorReady(SENSOR_REST_TIME)) {
        lastHum = bme.readHumidity();
        lastTemp = bme.readTemperature();
    }
    return lastHum;
}

bool BME280::isSensorReady(long interval) {
    long currentMillis = millis();

    if (currentMillis - lastMillis > interval) {
        lastMillis = currentMillis;
        return true;
    }

    return false;
}
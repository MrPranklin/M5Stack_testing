#include "DHT22_C.hpp"

#define DHTTYPE DHT22
#define SENSOR_REST_TIME 2000

DHT22_C::DHT22_C(short int pin) {
    dht = new DHT(pin, DHTTYPE);
    lastTemp = 0;
    lastHum = 0;
}

DHT22_C::~DHT22_C() {
    delete dht;
    dht = nullptr;
}

void DHT22_C::begin() {
    dht->begin();
}

float DHT22_C::readTemperature() {
    if (isSensorReady(SENSOR_REST_TIME)) {
        lastTemp = dht->readTemperature();
    }
    return lastTemp;
}

float DHT22_C::readHumidity() {
    if (isSensorReady(SENSOR_REST_TIME)) {
        lastHum = dht->readHumidity();
    }
    return lastHum;
}

bool DHT22_C::isSensorReady(long interval) {
    long currentMillis = millis();

    if (currentMillis - lastMillis > interval) {
        lastMillis = currentMillis;
        return true;
    }

    return false;
}

#include "DHT22_C.h"

#define DHTTYPE DHT22

DHT22_C::DHT22_C(short int pin)
{
  dht = new DHT(pin, DHTTYPE);
}

DHT22_C::~DHT22_C(){
  delete dht;
  dht = NULL;
}

void DHT22_C::begin()
{
  dht->begin();
}

float DHT22_C::read_temperature()
{
  return dht->readTemperature();
}

float DHT22_C::read_humidity()
{
  return dht->readHumidity();
}

bool DHT22_C::is_sensor_ready(long interval)
{
  long currentMillis = millis();

  if (currentMillis - lastMillis > interval)
  {
    lastMillis = currentMillis;
    Serial.println("Sensor is ready");
    return true;
  }

  return false;
}

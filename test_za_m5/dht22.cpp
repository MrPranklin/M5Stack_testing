#include <DHT.h>
#include "dht22.h"

#define DHTTYPE DHT22
#define DHTPIN 26

namespace dht22
{
DHT dht(DHTPIN, DHTTYPE);
long lastMillis = 0;

void begin()
{
  dht.begin();
}

float read_temperature()
{
  return dht.readTemperature();
}

float read_humidity()
{
  return dht.readHumidity();
}

bool is_sensor_ready(long interval)
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

} // namespace dht22

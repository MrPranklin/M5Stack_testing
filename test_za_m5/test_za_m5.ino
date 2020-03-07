#include <DHT.h>
#include <M5Stack.h>
#include "m5lcd.h"

#define DHTPIN 26
#define DHTTYPE DHT22

state_n::StateEnum state = state_n::temperature;
bool is_display_on = true;

long lastMillis = 0;
float temp = 0.0;
float hum = 0.0;

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  M5.begin(true, false, false, false);
  m5lcd::begin();

  Serial.begin(9600);
  Serial.println("Started");

  dht.begin();

  hum = read_humidity();
  temp = read_temperature();
  m5lcd::update_display(state, temp, hum);
}

void loop()
{
  check_buttons();

  if (is_display_on && is_sensor_ready(2000))
  {
    update_values(state);
  }
}

void update_values(state_n::StateEnum state)
{
  switch (state)
  {
  case state_n::temperature:
  {
    float new_temp = read_temperature();
    if (temp != new_temp)
    {
      temp = new_temp;
      m5lcd::update_display(state, temp, hum);
    }
    break;
  }
  case state_n::humidity:
  {
    float new_hum = read_humidity();
    if (hum != new_hum)
    {
      hum = new_hum;
      m5lcd::update_display(state, temp, hum);
    }
    break;
  }
  }
}

float read_temperature()
{
  return dht.readTemperature();
}

float read_humidity()
{
  return dht.readHumidity();
}

void set_state(state_n::StateEnum new_state)
{
  state = new_state;
  Serial.print("State set to: ");
  Serial.println(state);
  m5lcd::update_display(state, temp, hum);
  return;
}

void check_buttons()
{
  M5.update();

  if (M5.BtnA.wasPressed())
  {
    m5lcd::set_display_state(true);
    set_state(state_n::temperature);
  }
  else if (M5.BtnB.wasPressed())
  {
    m5lcd::set_display_state(true);
    set_state(state_n::humidity);
  }
  else if (M5.BtnC.wasPressed())
  {
    m5lcd::toggle_display();
  }

  return;
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
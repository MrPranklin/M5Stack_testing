#include <M5Stack.h>
#include "m5lcd.h"
#include "DHT22_C.h"

#define DHTPIN 26

state_n::StateEnum state = state_n::temperature;

DHT22_C dht22(DHTPIN);

float temp = 0.0;
float hum = 0.0;

void setup()
{
  M5.begin(true, false, false, false);
  m5lcd::begin();

  Serial.begin(9600);
  Serial.println("Started");

  dht22.begin();

  hum = dht22.read_humidity();
  temp = dht22.read_temperature();

  m5lcd::update_display(state, temp, hum);
}

void loop()
{
  check_buttons();

  if (m5lcd::is_display_on() && dht22.is_sensor_ready(2000))
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
    float new_temp = dht22.read_temperature();
    if (temp != new_temp)
    {
      temp = new_temp;
      m5lcd::update_display(state, temp, hum);
    }
    break;
  }
  case state_n::humidity:
  {
    float new_hum = dht22.read_humidity();
    if (hum != new_hum)
    {
      hum = new_hum;
      m5lcd::update_display(state, temp, hum);
    }
    break;
  }
  }
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
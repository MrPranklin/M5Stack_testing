#include <DHT.h>
#include <M5Stack.h>

#define DHTPIN 26
#define DHTTYPE DHT22

#define DEFAULT_BRIGHTNESS 100

enum State
{
  temperature,
  humidity
};

State state = State::temperature;
bool is_display_on = true;

long lastMillis = 0;
float temp = 0.0;
float hum = 0.0;

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  M5.begin(true, false, false, false);
  M5.Lcd.setTextSize(5);
  M5.Lcd.fillScreen(BLACK);

  Serial.begin(9600);
  Serial.println("Started");

  dht.begin();

  hum = read_humidity();
  temp = read_temperature();
  update_display(state);
}

void loop()
{
  check_buttons();

  if (is_display_on && is_sensor_ready(2000))
  {
    update_values(state);
  }
}

void update_values(State state)
{
  switch (state)
  {
  case State::temperature:
  {
    float new_temp = read_temperature();
    if (temp != new_temp)
    {
      temp = new_temp;
      update_display(state);
    }
    break;
  }
  case State::humidity:
  {
    float new_hum = read_humidity();
    if (hum != new_hum)
    {
      hum = new_hum;
      update_display(state);
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

void set_state(State new_state)
{
  state = new_state;
  Serial.print("State set to: ");
  Serial.println(state);
  update_display(state);
  return;
}

void toggle_display()
{
  if (is_display_on)
  {
    set_display_state(false);
  }
  else
  {
    set_display_state(true);
  }
}

void set_display_state(bool is_on)
{
  if (is_on)
  {
    is_display_on = true;
    M5.Lcd.setBrightness(DEFAULT_BRIGHTNESS);
  }
  else
  {
    is_display_on = false;
    M5.Lcd.setBrightness(0);
  }
}

void check_buttons()
{
  M5.update();

  if (M5.BtnA.wasPressed())
  {
    set_display_state(true);
    set_state(State::temperature);
  }
  else if (M5.BtnB.wasPressed())
  {
    set_display_state(true);
    set_state(State::humidity);
  }
  else if (M5.BtnC.wasPressed())
  {
    toggle_display();
  }

  return;
}

void update_display(State state)
{
  M5.Lcd.clear();

  switch (state)
  {
  case State::temperature:
    M5.Lcd.setCursor(100, 100);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.print(temp);
    break;

  case State::humidity:
    M5.Lcd.setCursor(100, 100);
    M5.Lcd.setTextColor(RED);
    M5.Lcd.print(hum);
    break;
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
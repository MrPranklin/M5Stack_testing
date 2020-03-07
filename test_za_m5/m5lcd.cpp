#include <M5Stack.h>
#include "m5lcd.h"

#define DEFAULT_BRIGHTNESS 100

namespace m5lcd
{
bool is_display_on = true;

void begin()
{
    M5.Lcd.setTextSize(5);
    M5.Lcd.fillScreen(BLACK);
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

void update_display(state_n::StateEnum state, float temp, float hum)
{
    M5.Lcd.clear();

    switch (state)
    {
    case state_n::temperature:
        M5.Lcd.setCursor(100, 100);
        M5.Lcd.setTextColor(GREEN);
        M5.Lcd.print(temp);
        break;

    case state_n::humidity:
        M5.Lcd.setCursor(100, 100);
        M5.Lcd.setTextColor(RED);
        M5.Lcd.print(hum);
        break;
    }

    return;
}

} // namespace m5lcd
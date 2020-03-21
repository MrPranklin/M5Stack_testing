#include "m5lcd.hpp"
#include <M5Stack.h>

#define DEFAULT_BRIGHTNESS 100

void update_battery_level(int level){
  M5.Lcd.setCursor(0,0);
  M5.Lcd.setTextColor(GREEN, BLACK);
  M5.Lcd.print(level);
  M5.Lcd.print("% ");
  return;
}

namespace m5lcd {
bool is_on = true;

void begin() {
    M5.Lcd.setTextSize(5);
    M5.Lcd.fillScreen(BLACK);
}

void toggle_display() {
    if (is_on) {
        set_display_state(false);
    } else {
        set_display_state(true);
    }
}

void set_display_state(bool new_state) {
    if (new_state) {
        is_on = true;
        M5.Lcd.setBrightness(DEFAULT_BRIGHTNESS);
    } else {
        is_on = false;
        M5.Lcd.setBrightness(0);
    }
}

void update_display(state_n::StateEnum state, float temp, float hum, int battery_level) {
    
    update_battery_level(battery_level);
    
    switch (state) {
        case state_n::temperature:
            M5.Lcd.setCursor(100, 100);
            M5.Lcd.setTextColor(GREEN, BLACK);
            M5.Lcd.print(temp);
            break;

        case state_n::humidity:
            M5.Lcd.setCursor(100, 100);
            M5.Lcd.setTextColor(RED, BLACK);
            M5.Lcd.print(hum);
            break;
    }

    return;
}

bool is_display_on() {
    return is_on;
}


}  // namespace m5lcd

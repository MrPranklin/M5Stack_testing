#include "m5lcd.hpp"
#include <M5Stack.h>

#define DEFAULT_BRIGHTNESS 50

void update_battery_level(int level) {
    if (level <= 100 && level > 75) {
        M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    } else if (level <= 75 && level > 50) {
        M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    } else if (level <= 50 && level > 25) {
        M5.Lcd.setTextColor(TFT_ORANGE, TFT_BLACK);
    } else {
        M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
    }

    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 0);

    M5.Lcd.printf("Battery: %d%%  ", level);
    return;
}

void showTemperature(float temp) {
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(30, 50);
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Lcd.print("Temperature:");

    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(70, 100);
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Lcd.printf("%.2f`C  ", temp);
}

void showTargetTemperature(float temp) {
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(30, 50);
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Lcd.print("Set target temp:");

    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(70, 100);
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Lcd.printf("%.2f`C  ", temp);
}

void showHumidity(float hum) {
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(30, 50);
    M5.Lcd.setTextColor(TFT_CYAN, TFT_BLACK);
    M5.Lcd.print("Humidity:");

    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(70, 100);
    M5.Lcd.setTextColor(TFT_CYAN, BLACK);
    M5.Lcd.printf("%.2f%%  ", hum);
}

namespace m5lcd {
    bool is_on = true;

    void begin() {
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setBrightness(DEFAULT_BRIGHTNESS);
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

    void update_display(state_n::StateEnum state, float temp, float hum, float targetTemp) {

        update_battery_level(M5.Power.getBatteryLevel());

        switch (state) {
            case state_n::temperature:
                showTemperature(temp);
                break;

            case state_n::humidity:
                showHumidity(hum);
                break;

            case state_n::setTargetTemperature:
                showTargetTemperature(targetTemp);
        }
        return;
    }

    void show_setting_up() {
        showMessage("Setting up...");
    }

    void clear() {
        M5.Lcd.clear();
    }

    bool is_display_on() {
        return is_on;
    }

    void showMessage(const char *message) {
        clear();

        M5.Lcd.setTextSize(3);
        M5.Lcd.setCursor(20, 100);
        M5.Lcd.setTextColor(RED, BLACK);
        M5.Lcd.print(message);
    }
}  // namespace m5lcd

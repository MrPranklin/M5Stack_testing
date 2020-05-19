#include <M5Stack.h>
#include <Arduino.h>

#include "m5lcd.hpp"

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
}

void showTemperature(float temp, bool isHeatControlEnabled, int heatingPercentage, int coolingPercentage) {
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(30, 50);
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Lcd.print("Temperature:");

    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(70, 100);
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Lcd.printf("%.2f`C  ", temp);

    if (isHeatControlEnabled) {

        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(70, 160);

        M5.Lcd.setTextColor(TFT_ORANGE, TFT_BLACK);


        M5.Lcd.printf("Heating: %d%%  ", heatingPercentage);

        M5.Lcd.setCursor(70, 190);
        M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);

        M5.Lcd.printf("Cooling: %d%%  ", coolingPercentage);

    } else {
        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(40, 175);
        M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);

        M5.Lcd.print("Heat control disabled");
    }
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

void showTime(String formattedTime) {
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(90, 0);
    M5.Lcd.print(formattedTime.c_str());
}

namespace m5lcd {
    bool is_on = true;

    void begin() {
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setBrightness(DEFAULT_BRIGHTNESS);
    }

    void toggle_display() {
        set_display_state(!is_on);
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

    void update_display(state_n::StateEnum state,
                        float temp,
                        float hum,
                        float targetTemp,
                        bool isHeatControlEnabled,
                        int heatingPercentage,
                        int coolingPercentage,
                        String formattedTime
    ) {
        update_battery_level(M5.Power.getBatteryLevel());
        showTime(formattedTime);

        switch (state) {
            case state_n::temperature:
                showTemperature(temp, isHeatControlEnabled, heatingPercentage, coolingPercentage);
                break;

            case state_n::humidity:
                showHumidity(hum);
                break;

            case state_n::setTargetTemperature:
                showTargetTemperature(targetTemp);
        }
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

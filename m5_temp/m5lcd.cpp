#include <M5Stack.h>
#include <Arduino.h>
#include <string>

#include "m5lcd.hpp"

#define DEFAULT_BRIGHTNESS 50

void showBatteryLevel(int level) {
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

    M5.Lcd.printf("%d%%  ", level);
}

void showTime(const String &formattedTime) {
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(90, 0);
    M5.Lcd.print(formattedTime.c_str());
}

void showTargetTemp(float targetTemp) {
    M5.Lcd.setTextSize(4);
    M5.Lcd.setCursor(5, 110);
    M5.Lcd.setTextColor(TFT_PINK, TFT_BLACK);
    M5.Lcd.printf("Trgt: %.2f`C  ", targetTemp);
}

void showHumidity(float hum) {
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(60, 160);
    M5.Lcd.setTextColor(TFT_CYAN, TFT_BLACK);
    M5.Lcd.printf("Hum: %3.2f%% ", hum);
}

void showCurrentTemp(float temp) {
    M5.Lcd.setTextSize(6);
    M5.Lcd.setCursor(40, 40);
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Lcd.printf("%.2f`C  ", temp);
}

void showHeatControlStatus(bool isHeatControlEnabled, int coolingPercentage, int heatingPercentage) {
    if (isHeatControlEnabled) {

        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(80, 210);
        M5.Lcd.setTextColor(TFT_ORANGE, TFT_BLACK);

        M5.Lcd.printf("C: %d%%  H: %d%%  ", coolingPercentage, heatingPercentage);

    } else {
        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(190, 210);
        M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);

        M5.Lcd.print("Heat control disabled");
    }
}

void showMainScreen(float temp, float targetTemp, float hum, bool isHeatControlEnabled, int heatingPercentage,
                    int coolingPercentage) {

    showCurrentTemp(temp);
    showTargetTemp(targetTemp);
    showHumidity(hum);
    showHeatControlStatus(isHeatControlEnabled, coolingPercentage, heatingPercentage);
}

void showSetTargetTemp(float targetTemp) {
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(30, 50);
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Lcd.print("Set target temp:");

    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(70, 100);
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Lcd.printf("%.2f`C  ", targetTemp);
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
                        const String &formattedTime
    ) {

        showBatteryLevel(M5.Power.getBatteryLevel());
        showTime(formattedTime);


        switch (state) {
            case state_n::temperature:
                showMainScreen(temp, targetTemp, hum, isHeatControlEnabled, heatingPercentage, coolingPercentage);
                break;

            case state_n::setTargetTemperature:
                showSetTargetTemp(targetTemp);
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

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

void
showTemperature(int brightness, bool isLightControlEnabled, int naturalLightPercentage, int artificialLightPercentage) {
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(30, 50);
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Lcd.print("Brightness:");

    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(70, 100);
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Lcd.printf("%d  ", brightness);

    if (isLightControlEnabled) {

        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(70, 160);

        M5.Lcd.setTextColor(TFT_ORANGE, TFT_BLACK);


        M5.Lcd.printf("Natural: %d%%  ", naturalLightPercentage);

        M5.Lcd.setCursor(70, 190);
        M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);

        M5.Lcd.printf("Artificial: %d%%  ", artificialLightPercentage);

    } else {
        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(40, 175);
        M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);

        M5.Lcd.print("Light control disabled");
    }
}

void showTargetBrightness(int brightness) {
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(30, 50);
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Lcd.print("Set target br:");

    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(70, 100);
    M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Lcd.printf("%d  ", brightness);
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
                        int brightness,
                        int currentTargetBrightness,
                        bool isLightControlEnabled,
                        int naturalLightPercentage,
                        int artificialLightPercentage
    ) {
        update_battery_level(M5.Power.getBatteryLevel());

        switch (state) {
            case state_n::brightness:
                showTemperature(brightness, isLightControlEnabled, naturalLightPercentage, artificialLightPercentage);
                break;

            case state_n::setTargetTemperature:
                showTargetBrightness(currentTargetBrightness);
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

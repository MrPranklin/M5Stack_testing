#include <M5Stack.h>
#include <Arduino.h>

#include "m5lcd.hpp"

#define DEFAULT_BRIGHTNESS 50

void updateBatteryLevel(int level) {
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

void showCurrent(bool isLightControlEnabled, int naturalLightPercentage, int artificialLightPercentage) {
    if (isLightControlEnabled) {

        M5.Lcd.setTextSize(3);
        M5.Lcd.setCursor(20, 80);

        M5.Lcd.setTextColor(TFT_ORANGE, TFT_BLACK);

        M5.Lcd.printf("Natural: %d%%  ", naturalLightPercentage);

        M5.Lcd.setCursor(20, 130);
        M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);

        M5.Lcd.printf("Artificial: %d%%  ", artificialLightPercentage);

    } else {
        M5.Lcd.setTextSize(3);
        M5.Lcd.setCursor(50, 100);
        M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);

        M5.Lcd.print("Light control");

        M5.Lcd.setCursor(90, 140);
        M5.Lcd.print("disabled");
    }
}

void showSetNaturalLightPercentage(int percentage) {
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(30, 50);
    M5.Lcd.setTextColor(TFT_ORANGE, TFT_BLACK);
    M5.Lcd.print("Set natural:");

    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(70, 100);
    M5.Lcd.setTextColor(TFT_ORANGE, TFT_BLACK);
    M5.Lcd.printf("%d%%  ", percentage);
}

void showSetArtificialLightPercentage(int percentage) {
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(30, 50);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.print("Set artificial:");

    M5.Lcd.setTextSize(5);
    M5.Lcd.setCursor(70, 100);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.printf("%d%%  ", percentage);
}

namespace m5lcd {
    bool _isOn = true;

    void begin() {
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setBrightness(DEFAULT_BRIGHTNESS);
    }

    void toggleDisplay() {
        setDisplayState(!_isOn);
    }

    void setDisplayState(bool new_state) {
        if (new_state) {
            _isOn = true;
            M5.Lcd.setBrightness(DEFAULT_BRIGHTNESS);
        } else {
            _isOn = false;
            M5.Lcd.setBrightness(0);
        }
    }

    void updateDisplay(state_n::StateEnum state,
                       bool isLightControlEnabled,
                       int naturalLightPercentage,
                       int artificialLightPercentage
    ) {
        updateBatteryLevel(M5.Power.getBatteryLevel());

        switch (state) {
            case state_n::showCurrent:
                showCurrent(isLightControlEnabled, naturalLightPercentage, artificialLightPercentage);
                break;

            case state_n::setNatural:
                showSetNaturalLightPercentage(naturalLightPercentage);
                break;

            case state_n::setArtificial:
                showSetArtificialLightPercentage(artificialLightPercentage);
                break;
        }
    }

    void showSettingUp() {
        showMessage("Setting up...");
    }

    void clear() {
        M5.Lcd.clear();
    }

    bool isDisplayOn() {
        return _isOn;
    }

    void showMessage(const char *message) {
        clear();

        M5.Lcd.setTextSize(3);
        M5.Lcd.setCursor(20, 100);
        M5.Lcd.setTextColor(RED, BLACK);
        M5.Lcd.print(message);
    }
}  // namespace m5lcd

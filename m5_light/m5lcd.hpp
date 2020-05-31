#ifndef M5LCD_H
#define M5LCD_H

#include "state_n.hpp"

namespace m5lcd {
    void begin();

    void toggleDisplay();

    void setDisplayState(bool is_on);

    void updateDisplay(state_n::StateEnum state,
                       bool isLightControlEnabled,
                       int naturalLightPercentage,
                       int artificialLightPercentage,
                       const String &formattedTime
    );

    void showSettingUp();

    void clear();

    bool isDisplayOn();

    void showMessage(const char *message);
}  // namespace m5lcd

#endif

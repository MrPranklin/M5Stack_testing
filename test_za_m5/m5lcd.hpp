#ifndef M5LCD_H
#define M5LCD_H

#include "state_n.hpp"

namespace m5lcd {
    void begin();

    void toggle_display();

    void set_display_state(bool is_on);

    void update_display(state_n::StateEnum state, float temp, float hum, float currentTargetTemp);

    void show_setting_up();

    void clear();

    bool is_display_on();

    void showMessage(const char *message);

    void showTargetTemperature(float currentTargetTemp);
}  // namespace m5lcd

#endif

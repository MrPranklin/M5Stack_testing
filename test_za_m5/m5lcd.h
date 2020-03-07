#include "state_n.h"

namespace m5lcd
{
void begin();

void toggle_display();

void set_display_state(bool is_on);

void update_display(state_n::StateEnum state, float temp, float hum);

bool is_display_on();
} // namespace m5lcd
#include<M5Stack.h>

namespace m5battery {
int get_battery_level() {
    Wire.beginTransmission(0x75);
    Wire.write(0x78);
    if (Wire.endTransmission(false) == 0 && Wire.requestFrom(0x75, 1)) {
        switch (Wire.read() & 0xF0) {
            case 0xE0:
                return 25;
            case 0xC0:
                return 50;
            case 0x80:
                return 75;
            case 0x00:
                return 100;
            default:
                return 0;
        }
    }
    return -1;
}
}  // namespace m5battery

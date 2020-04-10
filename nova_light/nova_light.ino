#include "ServoBlindController.hpp"

#define SERVO_PIN 4

ServoBlindController *servo;

void setup() {
    Serial.begin(115200);

    servo = new ServoBlindController(SERVO_PIN, 180);
}

void loop() {
    servo->open();
    delay(1000);
    servo->close();
    delay(1000);
    servo->setPercentage(50);
    delay(1000);
}
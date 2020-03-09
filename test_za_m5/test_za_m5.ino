#include <M5Stack.h>
#include "DHT22_C.h"
#include "m5lcd.h"
#include "ota.h"

#include <ESPmDNS.h>
#include <Update.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>

#define DHTPIN 26

state_n::StateEnum state = state_n::temperature;

DHT22_C dht22(DHTPIN);

float temp = 0.0;
float hum = 0.0;

void setup() {
    Serial.begin(115200);

    ota::begin();

    M5.begin(true, false, false, false);
    m5lcd::begin();

    dht22.begin();

    hum = dht22.read_humidity();
    temp = dht22.read_temperature();

    m5lcd::update_display(state, temp, hum);
    
    Serial.println("Setup finished");
}

void loop() {
    ota::handle_client();

    check_buttons();

    if (m5lcd::is_display_on() && dht22.is_sensor_ready(2000)) {
        update_values(state);
    }
}

void update_values(state_n::StateEnum state) {
    switch (state) {
        case state_n::temperature: {
            float new_temp = dht22.read_temperature();
            if (temp != new_temp) {
                temp = new_temp;
                m5lcd::update_display(state, temp, hum);
            }
            break;
        }
        case state_n::humidity: {
            float new_hum = dht22.read_humidity();
            if (hum != new_hum) {
                hum = new_hum;
                m5lcd::update_display(state, temp, hum);
            }
            break;
        }
    }
}

void set_state(state_n::StateEnum new_state) {
    state = new_state;
    Serial.print("State set to: ");
    Serial.println(state);
    m5lcd::update_display(state, temp, hum);
    return;
}

void check_buttons() {
    M5.update();

    if (M5.BtnA.wasPressed()) {
        m5lcd::set_display_state(true);
        set_state(state_n::temperature);
    } else if (M5.BtnB.wasPressed()) {
        m5lcd::set_display_state(true);
        set_state(state_n::humidity);
    } else if (M5.BtnC.wasPressed()) {
        m5lcd::toggle_display();
    }

    return;
}

#include <M5Stack.h>
#include "DHT22_C.hpp"
#include "m5lcd.hpp"
#include "ota.hpp"
#include "mqtt.hpp"
#include "m5battery.hpp"

#include <ESPmDNS.h>
#include <Update.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>

#define DHTPIN 26

const char* ssid = "Pranklin";
const char* password = "MrPranklin";
const char* host = "M5Stack";

state_n::StateEnum state = state_n::temperature;

DHT22_C dht22(DHTPIN);

float temp = 0.0;
float hum = 0.0;

void setup_wifi();
void reconnect(MQTT mqtt);

void setup() {
    Serial.begin(115200);
    Wire.begin(); // required for battery status
    M5.begin(true, false, false, false);

    setup_wifi();
    
    ota::begin();
    m5lcd::begin();
    dht22.begin();

    hum = dht22.read_humidity();
    temp = dht22.read_temperature();

    m5lcd::update_display(state, temp, hum, m5battery::get_battery_level());
    
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
                m5lcd::update_display(state, temp, hum, m5battery::get_battery_level());
            }
            break;
        }
        case state_n::humidity: {
            float new_hum = dht22.read_humidity();
            if (hum != new_hum) {
                hum = new_hum;
                m5lcd::update_display(state, temp, hum, m5battery::get_battery_level());
            }
            break;
        }
    }
}

void set_state(state_n::StateEnum new_state) {
    state = new_state;
    Serial.print("State set to: ");
    Serial.println(state);
    m5lcd::update_display(state, temp, hum, m5battery::get_battery_level());
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

void setup_wifi(){
    WiFi.begin(ssid, password);
    WiFi.setHostname(host);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    /*use mdns for host name resolution*/
    if (!MDNS.begin(host)) {  //http://m5stack.local
        Serial.println("Error setting up MDNS responder!");
        while (1) {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");
  }

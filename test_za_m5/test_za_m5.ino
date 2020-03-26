#include <ESPmDNS.h>
#include <M5Stack.h>
#include <Update.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include "DHT22_C.hpp"
#include "m5lcd.hpp"
#include "mqtt.hpp"
#include "ota.hpp"
#include "HeatControl.hpp"
#include "FanController.hpp"
#include "MockHeater.hpp"

#define DHTPIN 26

void callback(char *topic, byte *payload, unsigned int length);

void setup_wifi();

const char *ssid = "Pranklin";
const char *password = "MrPranklin";
const char *host = "M5Stack";

const IPAddress mqtt_server = IPAddress(192, 168, 0, 104);
const int mqtt_port = 1883;

state_n::StateEnum state = state_n::temperature;

long mqttLastMillis = 0;

DHT22_C dht22(DHTPIN);
WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);
//HeatControl *heatControl;

float temp = 0.0;
float hum = 0.0;

void setup() {
    Serial.begin(115200);
    M5.begin();
    M5.Power.begin();
    M5.Power.setPowerBoostKeepOn(false); // dont always output power

    m5lcd::begin();

    m5lcd::show_setting_up();

    ledcDetachPin(SPEAKER_PIN); // turn off speaker, less crackling
    setup_wifi();

    ota::begin();
    dht22.begin();

    hum = dht22.readHumidity();
    temp = dht22.readTemperature();

    m5lcd::clear();

    m5lcd::update_display(state, temp, hum);

//    Cooler *fanController = new FanController(20);
//    Heater *mockHeater = new MockHeater(19);
//
//    std::vector<Cooler*> coolers;
//    coolers.push_back(fanController);
//
//    std::vector<Heater*> heaters;
//    heaters.push_back(mockHeater);
//
//    heatControl = new HeatControl (&dht22, &coolers, &heaters);
//
//    heatControl->setTargetTemp(20);
//    heatControl->enable();

    Serial.println("Setup finished");
}

void loop() {
    ota::handle_client();

//    heatControl->update();

    if (!mqtt_client.loop()) {
        Serial.println("MQTT disconnected");
        mqtt::reconnect(mqtt_client);
    }

    check_buttons();

    if (m5lcd::is_display_on()) {
        update_values(state);
    }
}

void update_values(state_n::StateEnum state) {
    temp = dht22.readTemperature();
    hum = dht22.readHumidity();

    switch (state) {
        case state_n::temperature: {
            m5lcd::update_display(state, temp, hum);
            break;
        }
        case state_n::humidity: {
            m5lcd::update_display(state, temp, hum);
            break;
        }
    }

    if (mqtt::shouldUpdate(millis(), mqttLastMillis)) {
        mqtt::publish_temperature(mqtt_client, temp);
        mqtt::publish_humidity(mqtt_client, hum);
        mqttLastMillis = millis();
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
    M5.update(); // reads button state

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

void setup_wifi() {
    WiFi.begin(ssid, password);
    WiFi.setHostname(host);
    Serial.println("");

    // Wait for connection
    while (WiFi.isConnected()) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    WiFi.setAutoReconnect(true);

    /*use mdns for host name resolution*/
    if (!MDNS.begin(host)) {  //http://m5stack.local
        Serial.println("Error setting up MDNS responder!");
        while (1) {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");

    Serial.println("Setting up MQTT");
    mqtt_client.setServer(mqtt_server, mqtt_port);
    mqtt_client.setCallback(mqtt::callback);
    mqtt::reconnect(mqtt_client);
    Serial.println("MQTT setup done");
}

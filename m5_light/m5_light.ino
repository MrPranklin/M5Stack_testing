#include <ESPmDNS.h>
#include <M5Stack.h>
#include <Update.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include "m5lcd.hpp"
#include "mqtt.hpp"
#include "ota.hpp"
#include "LightControl.hpp"
#include "mqtt_topics.h"
#include "BrightnessSensor.hpp"
#include "Potentiometer.hpp"

#define POT_PIN 35

void callback(char *topic, byte *payload, unsigned int length);

void setup_wifi();

void check_buttons();

const char *ssid = "Pranklin";
const char *password = "MrPranklin";
const char *host = "M5Stack_light";

const IPAddress mqtt_server = IPAddress(192, 168, 0, 104);
const int mqtt_port = 1883;

state_n::StateEnum state = state_n::brightness;
state_n::StateEnum oldState = state_n::brightness;

long mqttLastMillis = 0;

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

LightControl *lightControl;
BrightnessSensor *sensor;

int brightness = 0;

void setup() {
    Serial.begin(115200);
    M5.begin();
    M5.Power.begin();
    M5.Power.setPowerBoostKeepOn(false); // dont always output power

    sensor = new Potentiometer(POT_PIN);
    lightControl = new LightControl(sensor, mqtt_client);
    lightControl->setTargetBrightness(22);

    m5lcd::begin();

    m5lcd::show_setting_up();

    ledcDetachPin(SPEAKER_PIN); // turn off speaker, less crackling
    setup_wifi();

    ota::begin();

    brightness = sensor->readBrightness();

    m5lcd::clear();

    m5lcd::update_display(
            state,
            brightness,
            lightControl->getTargetBrightness(),
            lightControl->isEnabled(),
            lightControl->getNaturalLightPercentage(),
            lightControl->getArtificialLightPercentage()
    );

    Serial.println("Setup finished");
    m5lcd::showMessage("Setup done");
    m5lcd::clear();
}

void loop() {
    ota::handle_client();

    if (!mqtt_client.loop()) {
        Serial.println("MQTT disconnected");
        mqtt::reconnect(mqtt_client);
    } else {

        if (lightControl->update()) {
            mqtt::updateNaturalLightPercentage(mqtt_client, lightControl->getNaturalLightPercentage());
            mqtt::updateArtificialLightPercentage(mqtt_client, lightControl->getArtificialLightPercentage());
        }

        brightness = sensor->readBrightness();

        check_buttons();

        if (m5lcd::is_display_on()) {
            m5lcd::update_display(
                    state,
                    brightness,
                    lightControl->getTargetBrightness(),
                    lightControl->isEnabled(),
                    lightControl->getNaturalLightPercentage(),
                    lightControl->getArtificialLightPercentage()
            );
        }

        if (mqtt::shouldUpdate(millis(), mqttLastMillis)) {

            if (lightControl->isEnabled()) {
                mqtt::updateNaturalLightPercentage(mqtt_client, lightControl->getNaturalLightPercentage());
                mqtt::updateArtificialLightPercentage(mqtt_client, lightControl->getArtificialLightPercentage());
            }

            mqtt::publishBrightness(mqtt_client, brightness);

            mqttLastMillis = millis();
        }
    }
}

void set_state(state_n::StateEnum new_state) {
    m5lcd::set_display_state(true);
    m5lcd::clear();

    state = new_state;

    Serial.print("State set to: ");
    Serial.println(state);

    m5lcd::update_display(
            state,
            brightness,
            lightControl->getTargetBrightness(),
            lightControl->isEnabled(),
            lightControl->getNaturalLightPercentage(),
            lightControl->getArtificialLightPercentage()
    );
}

void check_buttons() {
    M5.update(); // reads button state

    if (M5.BtnA.wasPressed()) {
        if (state == state_n::setTargetTemperature) {
            int curr = lightControl->incrementTargetBrightness(5);
            mqtt::updateTargetBrightness(mqtt_client, curr);
        } else {
            set_state(state_n::brightness);
        }
    } else if (M5.BtnB.wasPressed()) {
        if (state == state_n::setTargetTemperature) {
            int curr = lightControl->incrementTargetBrightness(-5);
            mqtt::updateTargetBrightness(mqtt_client, curr);
        }
    } else if (M5.BtnC.wasPressed()) {
        if (state == state_n::setTargetTemperature) {
            set_state(oldState);
        } else {
            oldState = state;
            set_state(state_n::setTargetTemperature);
        }
    }
}

void mqtt_callback(char *topic, byte *payload, unsigned int length) {
    payload[length] = '\0';

    Serial.print("MQTT: received ");
    Serial.print((char *) payload);
    Serial.print(" on ");
    Serial.println(topic);

    String strTopic = String((char *) topic);
    String pyld = String((char *) payload);

    if (strTopic == mqtt_command_light_control) {
        if (pyld == "ON") {
            lightControl->enable();
            mqtt::confirmLightControlOn(mqtt_client);

            lightControl->update();

            m5lcd::clear();
        } else if (pyld == "OFF") {
            lightControl->disable();
            mqtt::confirmLightControlOff(mqtt_client);
            m5lcd::clear();
        }
    } else if (strTopic == mqtt_command_target_brightness) {
        lightControl->setTargetBrightness(atoi((const char *) payload));
    }
}

void setup_wifi() {
    WiFi.begin(ssid, password);
    WiFi.setHostname(host);
    Serial.println("");

    // Wait for connection
    m5lcd::showMessage("Connecting WiFi");
    while (WiFi.isConnected()) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    m5lcd::clear();
    WiFi.setAutoReconnect(true);

    /*use mdns for host name resolution*/
    m5lcd::showMessage("Setting mdns");
    if (!MDNS.begin(host)) {  //http://m5stack.local
        Serial.println("Error setting up MDNS responder!");
        ESP.restart();
    }
    Serial.println("mDNS responder started");
    m5lcd::clear();

    Serial.println("Setting up MQTT");
    mqtt_client.setServer(mqtt_server, mqtt_port);
    mqtt_client.setCallback(mqtt_callback);
    mqtt::reconnect(mqtt_client);
    Serial.println("MQTT setup done");
}
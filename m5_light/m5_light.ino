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

void setup_wifi();

bool checkButtons();

const char *ssid = "Pranklin";
const char *password = "MrPranklin";
const char *host = "M5Stack_light";

const IPAddress mqtt_server = IPAddress(192, 168, 0, 24);
const int mqtt_port = 1883;

state_n::StateEnum state = state_n::showCurrent;
state_n::StateEnum oldState = state_n::showCurrent;

long mqttLastMillis = 0;

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

LightControl *lightControl;

void setup() {
    Serial.begin(115200);
    M5.begin();
    M5.Power.begin();
    M5.Power.setPowerBoostKeepOn(false); // dont always output power

    lightControl = new LightControl(mqtt_client);

    m5lcd::begin();

    m5lcd::showSettingUp();

    ledcDetachPin(SPEAKER_PIN); // turn off speaker, less crackling
    setup_wifi();

    ota::begin();

    m5lcd::clear();

    m5lcd::updateDisplay(
            state,
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
        if (checkButtons()) {
            if (lightControl->isEnabled()) {
                mqtt::updateNaturalLightPercentage(mqtt_client, lightControl->getNaturalLightPercentage());
                mqtt::updateArtificialLightPercentage(mqtt_client, lightControl->getArtificialLightPercentage());
            }
        } else if (mqtt::shouldUpdate(millis(), mqttLastMillis)) {
            if (lightControl->isEnabled()) {
                mqtt::updateNaturalLightPercentage(mqtt_client, lightControl->getNaturalLightPercentage());
                mqtt::updateArtificialLightPercentage(mqtt_client, lightControl->getArtificialLightPercentage());
            }

            mqttLastMillis = millis();
        }
        {}

        if (m5lcd::isDisplayOn()) {
            m5lcd::updateDisplay(
                    state,
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

            mqttLastMillis = millis();
        }
    }
}

void setState(state_n::StateEnum newState) {
    m5lcd::setDisplayState(true);
    m5lcd::clear();

    state = newState;

    Serial.print("State set to: ");
    Serial.println(state);

    m5lcd::updateDisplay(
            state,
            lightControl->isEnabled(),
            lightControl->getNaturalLightPercentage(),
            lightControl->getArtificialLightPercentage()
    );
}

bool checkButtons() {
    M5.update(); // reads button state

    if (M5.BtnA.wasPressed()) {
        if (state == state_n::showCurrent) {
            setState(state_n::setNatural);
        } else if (state == state_n::setArtificial) {
            int curr = lightControl->incrementArtificialLight(5);
        } else if (state == state_n::setNatural) {
            int curr = lightControl->incrementNaturalLight(5);
        }

        return true;
    } else if (M5.BtnB.wasPressed()) {
        if (state == state_n::showCurrent) {
            setState(state_n::setArtificial);
        } else if (state == state_n::setArtificial) {
            int curr = lightControl->incrementArtificialLight(-5);
        } else if (state == state_n::setNatural) {
            int curr = lightControl->incrementNaturalLight(-5);
        }

        return true;
    } else if (M5.BtnC.wasPressed()) {
        setState(state_n::showCurrent);
    }

    return false;
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

            mqtt::updateNaturalLightPercentage(mqtt_client, lightControl->getNaturalLightPercentage());
            mqtt::updateArtificialLightPercentage(mqtt_client, lightControl->getArtificialLightPercentage());

            m5lcd::clear();
        } else if (pyld == "OFF") {
            lightControl->disable();
            mqtt::confirmLightControlOff(mqtt_client);
            m5lcd::clear();
        }
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
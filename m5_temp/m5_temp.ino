#include <ESPmDNS.h>
#include <M5Stack.h>
#include <Update.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Timer.h>

#include "DHT22_C.hpp"
#include "m5lcd.hpp"
#include "mqtt.hpp"
#include "ota.hpp"
#include "HeatControl.hpp"
#include "mqtt_topics.h"
#include "BME280.hpp"

#define SET_TARGET_TEMP_DELAY 5000

void callback(char *topic, byte *payload, unsigned int length);

void setup_wifi();

void check_buttons();

const char *ssid = "Martin Ruter King";
const char *password = "volimkonje";
const char *host = "M5Stack temp";

const IPAddress mqtt_server = IPAddress(192, 168, 0, 24);
const int mqtt_port = 1883;

state_n::StateEnum state = state_n::temperature;
state_n::StateEnum oldState = state_n::temperature;

long mqttLastMillis = 0;

BME280 bme;

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

HeatControl *heatControl;

float temp = 0.0;
float hum = 0.0;

Timer setTargetTempTimer;
int setTargetTempEventId;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    M5.begin();
    M5.Power.begin();
    M5.Power.setPowerBoostKeepOn(false); // dont always output power

    m5lcd::begin();

    m5lcd::show_setting_up();

    ledcDetachPin(SPEAKER_PIN); // turn off speaker, less crackling
    setup_wifi();

    ota::begin();

    timeClient.begin();

    bme.begin();

    heatControl = new HeatControl(&bme, mqtt_client);

    Serial.println("Setup finished");
    m5lcd::showMessage("Setup done");
    m5lcd::clear();
}

void loop() {
    ota::handle_client();
    timeClient.update();
    setTargetTempTimer.update();

    if (!mqtt_client.loop()) {
        Serial.println("MQTT disconnected");
        mqtt::reconnect(mqtt_client);
    } else {
        temp = bme.readTemperature();
        hum = bme.readHumidity();

        if (heatControl->update()) {
            mqtt::updateHeatingPercentage(mqtt_client, heatControl->getHeatingPercentage());
            mqtt::updateCoolingPercentage(mqtt_client, heatControl->getCoolingPercentage());
        }

        check_buttons();

        if (m5lcd::is_display_on()) {
            m5lcd::update_display(
                    state,
                    temp,
                    hum,
                    heatControl->getTargetTemp(),
                    heatControl->isEnabled(),
                    heatControl->getHeatingPercentage(),
                    heatControl->getCoolingPercentage(),
                    timeClient.getFormattedTime()
            );
        }

        if (mqtt::shouldUpdate(millis(), mqttLastMillis)) {

            if (heatControl->isEnabled()) {
                mqtt::updateHeatingPercentage(mqtt_client, heatControl->getHeatingPercentage());
                mqtt::updateCoolingPercentage(mqtt_client, heatControl->getCoolingPercentage());
            }

            mqtt::publish_temperature(mqtt_client, temp);
            mqtt::publish_humidity(mqtt_client, hum);

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
            temp,
            hum,
            heatControl->getTargetTemp(),
            heatControl->isEnabled(),
            heatControl->getHeatingPercentage(),
            heatControl->getCoolingPercentage(),
            timeClient.getFormattedTime()
    );
}

void stopTimer() {
    setTargetTempTimer.stop(setTargetTempEventId);
}

void restoreDefaultState() {
    set_state(state_n::temperature);
    stopTimer();
}

void initTimer() {
    setTargetTempTimer.after(SET_TARGET_TEMP_DELAY, restoreDefaultState);
}

void resetTimer() {
    stopTimer();
    initTimer();
}

void check_buttons() {
    M5.update(); // reads button state

    if (M5.BtnA.wasPressed()) { // target+=0.5
        if (state == state_n::setTargetTemperature) {
            resetTimer();
            float curr = heatControl->incrementTargetTemp(0.5);
            mqtt::updateTargetTemp(mqtt_client, curr);
        } else {
            set_state(state_n::setTargetTemperature);
            initTimer();
        }
    } else if (M5.BtnB.wasPressed()) { // OK
        if (state == state_n::setTargetTemperature) {
            stopTimer();
            set_state(state_n::temperature);
        }
    } else if (M5.BtnC.wasPressed()) { // target-=0.5
        if (state == state_n::setTargetTemperature) {
            resetTimer();
            float curr = heatControl->incrementTargetTemp(-0.5);
            mqtt::updateTargetTemp(mqtt_client, curr);
        } else {
            initTimer();
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

    if (strTopic == mqtt_command_heat_control) {
        if (pyld == "ON") {
            heatControl->enable();
            mqtt::confirmHeatControlOn(mqtt_client);

            heatControl->update();

            m5lcd::clear();
        } else if (pyld == "OFF") {
            heatControl->disable();
            mqtt::confirmHeatControlOff(mqtt_client);
            m5lcd::clear();
        }
    } else if (strTopic == mqtt_command_target_temp) {
        heatControl->setTargetTemp(atof((const char *) payload));
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
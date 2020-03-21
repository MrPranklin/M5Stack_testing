#include <ESPmDNS.h>
#include <M5Stack.h>
#include <Update.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include "DHT22_C.hpp"
#include "m5battery.hpp"
#include "m5lcd.hpp"
#include "mqtt.hpp"
#include "ota.hpp"

#define DHTPIN 26

void callback(char* topic, byte* payload, unsigned int length);
void setup_wifi();
void reconnect(MQTT mqtt);

const char* ssid = "Pranklin";
const char* password = "MrPranklin";
const char* host = "M5Stack";

const IPAddress mqtt_server = IPAddress(192, 168, 0, 102);
const int mqtt_port = 1883;
const char* mqtt_in_topic = "M5/switch1";
const char* mqtt_out_topic = "M5/out";
const char* mqtt_client_id = "M5Stack_client";

state_n::StateEnum state = state_n::temperature;

DHT22_C dht22(DHTPIN);
WiFiClient wifi_client;
PubSubClient mqtt(wifi_client);

float temp = 0.0;
float hum = 0.0;

void setup() {
    Serial.begin(115200);
    Wire.begin();  // required for battery status
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

    if (!mqtt.loop()) {
        Serial.println("MQTT disconnected");
        reconnect(mqtt);
    }
    
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

void setup_wifi() {
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

    Serial.println("Setting up MQTT");
    mqtt.setServer(mqtt_server, mqtt_port);
    mqtt.setCallback(callback);
    reconnect(mqtt);
    Serial.println("MQTT setup done");
}

void callback(char* topic, byte* payload, unsigned int length) {
    payload[length] = '\0';
    String strTopic = String((char*)topic);
    String pyld = String((char*)payload);
    if (strTopic == mqtt_in_topic) {
        if (pyld == "ON") {
            Serial.println("ON");
        } else {
            Serial.println("OFF");
        }
    }
}

void reconnect(PubSubClient mqtt) {
    while (!mqtt.connected()) {
        Serial.print("Attempting MQTT connection...");

        if (mqtt.connect(mqtt_client_id)) {
            Serial.print("connected as ");
            Serial.println(mqtt_client_id);
            // Once connected, publish an announcement...
            mqtt.publish(mqtt_out_topic, "This is M5Stack");
            // ... and resubscribe
            mqtt.subscribe(mqtt_in_topic);
            Serial.print("Subscribed to ");
            Serial.println(mqtt_in_topic);
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqtt.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

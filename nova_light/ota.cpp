#include <ArduinoOTA.h>
#include "ota.hpp"

#define HOSTNAME "nova_cooling_heating"
#define PASSWORD "nova_cooling_heating"

namespace ota {

    void begin() {
        ArduinoOTA.setHostname(HOSTNAME);

        ArduinoOTA.setPassword((const char *) PASSWORD);

        ArduinoOTA.onStart([]() {
            Serial.println("Start");
        });
        ArduinoOTA.onEnd([]() {
            Serial.println("\nEnd");
        });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        });
        ArduinoOTA.onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR) Serial.println("End Failed");
        });
        ArduinoOTA.begin();
    }

    void handleClient() {
        ArduinoOTA.handle();
        delay(1);
    }
}  // namespace ota

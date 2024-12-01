#include <WiFi.h>
#include "wifi_utils.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";

void setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.print("Conectando ao Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi conectado");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

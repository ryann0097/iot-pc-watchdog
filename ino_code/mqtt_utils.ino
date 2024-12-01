#include <PubSubClient.h>
#include "mqtt_utils.h"

extern PubSubClient client;

void setupMQTT() {
    client.setServer("io.adafruit.com", 1883);
    client.setCallback(callbackMQTT);
}

void reconnectMQTT() {
    while (!client.connected()) {
        String clientId = "ESP32-Simulacao";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
            client.subscribe("ryansilv68/feeds/controle");
        } else {
            delay(5000);
        }
    }
}

void publicarDadosMQTT(float cpu, float temp, float mem, String log) {
    publishCpuUsage(cpu);
    publishCpuTemp(temp);
    publishMemUsage(mem);
    publishLog(log);
}

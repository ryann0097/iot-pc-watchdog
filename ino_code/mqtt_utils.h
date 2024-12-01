#ifndef MQTT_UTILS_H
#define MQTT_UTILS_H

void setupMQTT();
void reconnectMQTT();
void publicarDadosMQTT(float cpu, float temp, float mem, String log);

#endif

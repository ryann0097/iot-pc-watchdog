#include "hardware_utils.h"

void setupHardware() {
    pinMode(BUZZER, OUTPUT);
    pinMode(HCSR04_TRIG, OUTPUT);
    pinMode(HCSR04_ECHO, INPUT);
}

float verificarDistanciaUsuario() {
    digitalWrite(HCSR04_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(HCSR04_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(HCSR04_TRIG, LOW);

    long duracaoPulso = pulseIn(HCSR04_ECHO, HIGH);
    return duracaoPulso * VELOC_SOM / 2;
}

void controlarAlarme(int estado, float distancia) {
    int frequencia = (estado == 4) ? 4000 : (estado == 3) ? 2000 : (estado == 2) ? 1000 : 0;
    if (frequencia == 0) {
        noTone(BUZZER);
    } else {
        tone(BUZZER, frequencia);
    }
}

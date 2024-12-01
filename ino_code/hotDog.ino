#include "wifi_utils.h"
#include "mqtt_utils.h"
#include "spiffs_utils.h"
#include "hardware_utils.h"
#include "risk_analysis.h"

unsigned long lastMillis = 0;

void setup() {
    Serial.begin(115200);
    setupWiFi();
    setupMQTT();
    setupSPIFFS();
    setupHardware();
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - lastMillis >= INTERVALO) {
        lastMillis = currentMillis;

        String dadosScript = obterDadosPC();
        if (!dadosScript.isEmpty()) {
            processarDados(dadosScript);
        }

        String log = formatarLog(cpuUsage, temperatura, memUsage);

        if (!client.connected()) {
            reconnectMQTT();
        }
        client.loop();

        if (WiFi.isConnected()) {
            publicarDadosMQTT(cpuUsage, temperatura, memUsage, log);
        } else {
            salvarNoSPIFFS(log);
        }

        atualizarHistoricoRisco(cpuUsage, temperatura, memUsage);
        int estadoAtual = avaliarRisco();
        float distancia = verificarDistanciaUsuario();
        controlarAlarme(estadoAtual, distancia);
    }
}

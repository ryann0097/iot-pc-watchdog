#include <SPIFFS.h>
#include "spiffs_utils.h"

void setupSPIFFS() {
    if (!SPIFFS.begin(true)) {
        Serial.println("Erro ao inicializar SPIFFS");
    } else {
        Serial.println("SPIFFS inicializado");
    }
}

void salvarNoSPIFFS(String dados) {
    File file = SPIFFS.open("/logs.txt", FILE_APPEND);
    if (file) {
        file.println(dados);
        file.close();
    }
}

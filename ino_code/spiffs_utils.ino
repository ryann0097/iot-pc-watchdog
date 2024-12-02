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

bool arquivoEstaCheio(size_t tamanhoMaxArquivo) {
    File file = SPIFFS.open("/logs.txt", FILE_READ);
    if (!file) {
        Serial.println("Erro ao abrir o arquivo para verificar tamanho.");
        return false; 
    }
    bool cheio = file.size() > tamanhoMaxArquivo;
    file.close();
    return cheio;
}

void removerLogMaisAntigo() {
    File fileRead = SPIFFS.open("/logs.txt", FILE_READ);
    if (!fileRead) {
        Serial.println("Erro ao abrir o arquivo de logs para leitura.");
        return;
    }

    String logs = "";
    bool primeiraLinhaRemovida = false;

    while (fileRead.available()) {
        String linha = fileRead.readStringUntil('\n');
        if (primeiraLinhaRemovida) {
            logs += linha + "\n"; // Adiciona todas as linhas, exceto a primeira
        } else {
            primeiraLinhaRemovida = true;
        }
    }
    fileRead.close();

    File fileWrite = SPIFFS.open("/logs.txt", FILE_WRITE);
    if (!fileWrite) {
        Serial.println("Erro ao abrir o arquivo de logs para escrita.");
        return;
    }
    fileWrite.print(logs);
    fileWrite.close();

    Serial.println("Log mais antigo removido.");
}

void gerenciarLogsSPIFFS(const String& novoLog, size_t tamanhoMaxArquivo) {
    if (arquivoEstaCheio("/logs.txt", tamanhoMaxArquivo)) {
        removerLogMaisAntigo("/logs.txt");
    }
    salvarNoSPIFFS(novoLog);
}

#include "risk_analysis.h"

#define CICLOS 3
#define LIMITE_ALERTA 70

int historicoCpu[CICLOS] = {0};
int historicoMemoria[CICLOS] = {0};
int historicoTemperatura[CICLOS] = {0};

int indiceHistorico = 0;

void atualizarHistoricoRisco(float cpu, float temp, float mem) {
    historicoCpu[indiceHistorico] = cpu;
    historicoTemperatura[indiceHistorico] = temp;
    historicoMemoria[indiceHistorico] = mem;
    indiceHistorico = (indiceHistorico + 1) % CICLOS; // Atualiza índice circular
}

bool mediaElevada(int* historico, int limite) {
    int soma = 0;
    for (int i = 0; i < CICLOS; i++) {
        soma += historico[i];
    }
    return (soma / CICLOS) >= limite;
}

int avaliarRisco() {
    bool cpuAlta = mediaElevada(historicoCpu, LIMITE_ALERTA);
    bool memoriaAlta = mediaElevada(historicoMemoria, LIMITE_ALERTA);
    bool temperaturaAlta = mediaElevada(historicoTemperatura, LIMITE_ALERTA);

    if (temperaturaAlta && !cpuAlta && !memoriaAlta) {
        return 4; // Anomalia térmica
    } else if (cpuAlta && memoriaAlta && temperaturaAlta) {
        return 3; // Sobrecarga constante
    } else if (cpuAlta || memoriaAlta || temperaturaAlta) {
        return 2; // Alerta: Pico detectado
    } else {
        return 1; // Estado: Normal
    }
}

String formatarLog(float cpu, float temp, float mem) {
    return "[Cpu Usage] " + String(cpu) + 
           " [Cpu Temp] " + String(temp) + 
           " [Mem Usage] " + String(mem);
}

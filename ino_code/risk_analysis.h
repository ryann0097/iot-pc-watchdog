#ifndef RISK_ANALYSIS_H
#define RISK_ANALYSIS_H

void atualizarHistoricoRisco(float cpu, float temp, float mem);
int avaliarRisco();
bool mediaElevada(int* historico, int limite);
String formatarLog(float cpu, float temp, float mem);

#endif

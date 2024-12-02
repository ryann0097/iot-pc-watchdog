#ifndef SPIFFS_UTILS_H
#define SPIFFS_UTILS_H

void setupSPIFFS();
void salvarNoSPIFFS(String dados);
bool arquivoEstaCheio(size_t tamanhoMaxArquivo);
void removerLogMaisAntigo();
void gerenciarLogsSPIFFS(const String& novoLog, size_t tamanhoMaxArquivo);

#endif

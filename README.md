# PC Watchdog: Monitoramento de Temperatura e Desempenho de CPU

## Motivação

<p>"PC Watchdog" tem como objetivo, semelhantemente a um cão de guarda, alertar sobre potenciais riscos ao seu computador, usando scripts em Python para isso e tendo como auxiliar um componente de IoT muito popular, o ESP32. O projeto consegue rastreiar a temperatura, o uso de CPU e o uso de memória RAM, fornecendo dados em tempo real para identificar e previnir problemas relacionados ao desempenho e à saúde do computador.</p>
<p>O sistema é projetado para detectar falhas iminentes, como superaquecimento e sobrecarga de recursos, além de possíveis anomalias de componentes, oferecendo uma abordagem proativa e preventiva para a manutenção e otimização do computador. Os dados coletados são processados e enviados para a nuvem, onde podem ser visualizados e analisados, permitindo o monitoramente remoto e alertas em casos de anomalias.</p>

---

## Funcionalidades principais:
- Monitoramento da temperatura do sistema e componentes críticos;
- Acompanhamento do uso de CPU e memória RAM;
- Envio de alertas para detectar problemas antes que se tornem críticos;
- Integração em IoT para a visualização dos dados na nuvem com Adafruit IO.

---

## Especificações de Projeto

### Componentes:
- **ESP32**: Recebimento de scripts, controle de componentes.
- **Sensor Ultrassônico**: Verificação de presença de usuário.

### Tecnologias Usadas:

#### Conectividade:
- **Adafruit IO**: Armazenamento em nuvem e acompanhamento em tempo real.
- **MQTT Broker**: Para se conectar com o Adafruit IO.

#### Armazenamento:
- **SPIFFS**: Armazenamento de logs do sistema.

#### Scripts:
- **Python**: Repassagem de argumentos do PC para o ESP32.

---

## Metodologia do Projeto

### Coleta de Dados

#### Dados Internos (captura através de `psutil`):
- **Temperatura de CPU**
- **Uso de CPU**
- **Uso de Memória**

### Processamento de Dados

#### ESP32 (Placa de Desenvolvimento):
- Receber dados do **HC-SR04** (sensor ultrassônico).
- Formatar e enviar dados para o **Adafruit.io** ou **SPIFFS**.

#### Python:
- Rodar scripts para coletar dados internos em horários regulares.
- Enviar os dados de alguma maneira para o **ESP32** ou diretamente para o **Adafruit.io**.

### Análise Qualitativa

A análise da temperatura e desempenho da CPU é dividida em níveis de risco:

- **Nível 1**: Uso de CPU normal, com temperatura normal.
- **Nível 2**: Uso de CPU normal, com temperaturas quase críticas.
- **Nível 3**: Uso de CPU quase crítico, com temperaturas quase críticas.
- **Nível 4**: Uso de CPU crítico, com temperaturas altas.

### Análise Quantitativa ou Temporal

- **Período de Coleta**: 2 minutos entre o envio dos dados do script para o **ESP32**.
- **Heurística Básica**: Análise simples de picos utilizando três pontos de análise para determinar se o desempenho é constante ou se há um pico de uso.
  
---

## Uso de Dados e Armazenamento

### Armazenamento no SPIFFS (Offline):
- **Logs**: Armazenamento de temperatura, uso de CPU e eventos críticos.

### Adafruit IO (Nuvem):
- **Dashboard em Tempo Real**: Exibição de dados ao vivo.
- **Histórico para Análise Futura**: Armazenamento e visualização dos dados históricos.

---

## Fluxo do Projeto

1. **Conexão com a Rede e Broker**: O sistema se conecta à internet e ao broker MQTT.
2. **Coleta e Processamento de Dados**: O sistema coleta dados do computador e dos sensores de proximidade.
3. **Análise de Risco**: Avalia os dados para identificar anomalias ou picos de uso.
4. **Publicação e Alerta**: Envia os dados para a nuvem via MQTT e ajusta o alarme sonoro conforme o nível de risco.
5. **Gestão de Falhas**: Armazena dados localmente no SPIFFS caso a conexão MQTT esteja indisponível e tenta se reconectar.
6. **Recebimento de Comandos**: Recebe comandos remotos via MQTT, como resetar os valores de monitoramento.


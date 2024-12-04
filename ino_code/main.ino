#include <WiFi.h>
#include <PubSubClient.h>
#include <SPIFFS.h>


/** ========[ Funcões e Setup de Componentes ]==========
 * Definindo o setup do buzzer (alarme único baseado na questão do nível de risco)
 * Definição do HC-SR04 para verificar proximidade de usuário
 * =====================================================
 */

#define BUZZER 5
#define HCSR04_TRIG 18
#define HCSR04_ECHO 19
#define VELOC_SOM 0.034
#define INTERVALO 120000

float verificarDistanciaUsuario(){
  digitalWrite(HCSR04_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(HCSR04_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(HCSR04_TRIG, LOW);

  long duracaoPulso = pulseIn(HCSR04_ECHO, HIGH);
  float distanciaCm = duracaoPulso * VELOC_SOM / 2;

  return distanciaCm;
}

/** ===============[ Variáveis de Ciclo ]===============
 *  Variáveis para avaliação da situação por tempo
 * =====================================================
 */

#define CICLOS 3
int historicoCPU[CICLOS] = {0};
int historicoMemoria[CICLOS] = {0};
int historicoTemperatura[CICLOS] = {0};

int indiceHistorico = 0;

/** ===============[ Funções de Ciclo ]===============
 *  Variáveis para avaliação da situação por tempo
 * =====================================================
 */

void atualizarHistorico(int* historico, int valor) {
  historico[indiceHistorico] = valor;
  indiceHistorico = (indiceHistorico + 1) % CICLOS;  // Atualiza o índice de forma circular
}

int avaliarRisco() {
  bool cpuAlta = mediaElevada(historicoCPU, 70);
  bool memoriaAlta = mediaElevada(historicoMemoria, 70);
  bool temperaturaAlta = mediaElevada(historicoTemperatura, 70);

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

bool mediaElevada(int* historico, int limite) {
  int soma = 0;
  for (int i = 0; i < CICLOS; i++) {
    soma += historico[i];
  }
  return (soma / CICLOS) >= limite;
}


/** ==================[ Adafruit IO ]====================
 * Configurações para o Adafruit IO
 * ============================================
 */
#define IO_USERNAME  "ryansilv68"
#define IO_KEY       "aio_dQZI64bfLMOtQDrgdoJfHa7kHEpb"
const char* ssid = "ryann0097";
const char* password = "rdss0973";

/** ==================[ MQTT Protocol ]===================
 * Configurações do MQTT
 * ============================================
 */
const char* mqttserver = "io.adafruit.com";
const int mqttport = 1883;
const char* mqttUser = IO_USERNAME;
const char* mqttPassword = IO_KEY;

// Objetos de Comunicação
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

/** =====================================================
 * Inicialização de variáveis
 * ============================================
 */
float temperatura = 0.0; 
float cpuUsage = 0.0;     
float memUsage = 0.0;     

/** ==================[ Wifi Setup ]====================
 * Configuração de Wi-Fi
 * ============================================
 */
void setup_wifi() {
  delay(10);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

/** ==================[ Callback ]======================
 * Função de callback para mensagens recebidas via MQTT
 * ============================================
 */
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");

  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();

  if (String(topic) == "ryansilv68/feeds/controle") {
    if (messageTemp == "reset") {
      temperatura = 25.0;
      cpuUsage = 20.0;
      memUsage = 30.0;
      Serial.println("Valores resetados.");
    }
  }
}

/** ==================[ Reconnect ]======================
 * Função para reconectar ao MQTT Broker
 * ============================================
 */
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    String clientId = "ESP32-Simulacao";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword)) {
      Serial.println("Conectado");
      client.subscribe("ryansilv68/feeds/controle");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5s");
      delay(5000);
    }
  }
}

/** ==================[ Funções de Publicação do MQTT ]=====
 * Funções para publicar dados do sistema via MQTT
 * ============================================
 */
void publishCpuTemp(float value) {
    char s_value[8];
    dtostrf(value, 1, 2, s_value);
    client.publish("ryansilv68/feeds/pc-watchdog-project-pilot.temperature", s_value);
}

void publishCpuUsage(float value) {
    char s_value[8];
    dtostrf(value, 1, 2, s_value);
    client.publish("ryansilv68/feeds/pc-watchdog-project-pilot.cpu-usage", s_value);
}

void publishMemUsage(float value) {
    char s_value[8];
    dtostrf(value, 1, 2, s_value);
    client.publish("ryansilv68/feeds/pc-watchdog-project-pilot.mem-usage", s_value);
}

void publishLog(String dados) {
    char log_cstr[60];
    String log = dados;
    strcpy(log_cstr, log.c_str());
    client.publish("ryansilv68/feeds/pc-watchdog-project-pilot.logs", log_cstr);
}

/** ==================[ SPIFFS - Armazenamento Local ]======
 * Funções para salvar dados em SPIFFS caso não haja conexão MQTT
 * ============================================
 */

void salvarNoSPIFFS(String dados) {
  File file = SPIFFS.open("/logs.txt", FILE_APPEND);
  if (file) {
    file.println(dados);
    file.close();
    Serial.println("Log salvo no SPIFFS");
  } else {
    Serial.println("Erro ao salvar log no SPIFFS");
  }
}

/** ==================[ Receber Dados do PC ]=============
 * Funções para receber dados via serial do PC e processá-los
 * ============================================
 */
String obterDados() {
    if (Serial.available() > 0) {
        return Serial.readStringUntil('\n');
    }
    return "";
}

void processarDados(String dados) {
    int primeiroEspaco = dados.indexOf(',');
    int segundoEspaco = dados.indexOf(',', primeiroEspaco + 1);

    String uso_cpu = dados.substring(0, primeiroEspaco);
    String temperatura_cpu = dados.substring(primeiroEspaco + 1, segundoEspaco);
    String uso_memoria = dados.substring(segundoEspaco + 1);

    cpuUsage = uso_cpu.toFloat();
    temperatura = temperatura_cpu.toFloat();
    memUsage = uso_memoria.toFloat();
}

/** ==================[ Perturbar ]==========
 * Funções para alertar temperaturas criticas
 * ============================================
 */
void perturbaPerto(float temperaturaAtual) {
    if (temperaturaAtual > temperaturaCritica) {
        digitalWrite(LED_PIN, HIGH); 
        tone(BUZZER_PIN, 2000);       
    } else {
        digitalWrite(LED_PIN, LOW);   
        noTone(BUZZER_PIN);           
    }
}

void perturbaLonge(float temperaturaAtual){
  if(temperaturaAtual > temperaturaCritica){
     if (temperaturaAtual > 65.0) {
        String mensagem = "ALERTA: Temperatura crítica detectada! (" + String(temperaturaAtual) + " °C)";
        char mensagemCstr[100];
        mensagem.toCharArray(mensagemCstr, 100); 
        client.publish("ryansilv68/feeds/pc-watchdog-project-pilot.alertas", mensagemCstr);
        Serial.println("Alerta enviado ao Adafruit IO: " + mensagem); 
  }
}

/** ==================[ Setup e Loop Principal ]==========
 * Funções para configurar e executar o ciclo principal
 * ============================================
 */
void setup() {
  setup_wifi();
  client.setServer(mqttserver, mqttport); 
  client.setCallback(callback);

  if (!SPIFFS.begin(true)) {
    Serial.println("Erro ao montar SPIFFS");
    return;
  }
  Serial.println("SPIFFS iniciado");

  // Setup dos componentes
  pinMode(BUZZER, OUTPUT);
  pinMode(HCSR04_TRIG, OUTPUT);
  pinMode(HCSR04_ECHO, INPUT);
}

unsigned long lastMillis = 0;  // Variável para armazenar o tempo do último ciclo

void loop() {
  unsigned long currentMillis = millis();  // Tempo atual
  int frequenciaAlarme = 0;

  // Preferi trocar delay por isso aqui, para não interromper o buzzer
  if (currentMillis - lastMillis >= INTERVALO) {
  
    lastMillis = currentMillis;

    // Receber dados do script do PC.
    String dadosScript = obterDados();
    if (!dadosScript.isEmpty()) {
      processarDados(dadosScript);
    }

    String dados = "[Cpu Usage] " + String(cpuUsage) + " [Cpu Temp] " + String(temperatura) + " [Mem Usage] " + String(memUsage);

    // Verificar conexão com o MQTT Broker
    if (!client.connected()) {
      reconnect();
    }
    client.loop();

    // Se tiver internet, publicar dados no MQTT.
    
      publishCpuTemp(temperatura);
      publishCpuUsage(cpuUsage);
      publishMemUsage(memUsage);
      publishLog(dados);
   
      salvarNoSPIFFS(dados);
 

    // Aqui é o sistema de monitoramento local:
    atualizarHistorico(historicoCPU, cpuUsage);
    atualizarHistorico(historicoTemperatura, temperatura);
    atualizarHistorico(historicoMemoria, memUsage);

    int estadoAtual = avaliarRisco();
    float distanciaUsuario = verificarDistanciaUsuario();

    switch (estadoAtual) {
      case 1:
        frequenciaAlarme = 0;
        break;
      case 2:
        frequenciaAlarme = 1000;
        break;
      case 3:
        frequenciaAlarme = 2000;
        break;
      case 4:
        frequenciaAlarme = 4000;
        break;
    }
  }

  if(frequenciaAlarme == 0){
    noTone(BUZZER);
  } else {
    tone(BUZZER, frequenciaAlarme);
  }

}


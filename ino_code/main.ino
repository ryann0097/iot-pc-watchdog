#include <WiFi.h>
#include <PubSubClient.h>
#include <SPIFFS.h>

/** ==================[ Adafruit IO ]====================
 * Configurações para o Adafruit IO
 * ============================================
 */
#define IO_USERNAME  "ryansilv68"
#define IO_KEY       "aio_dQZI64bfLMOtQDrgdoJfHa7kHEpb"
const char* ssid = "Wokwi-GUEST";
const char* password = "";

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
    strcpy(log_cstr, log);
    client.publish("ryansilv68/feeds/pc-watchdog-project-pilot.logs", log_cstr);
}

/** ==================[ SPIFFS - Armazenamento Local ]======
 * Funções para salvar dados em SPIFFS caso não haja conexão MQTT
 * ============================================
 */



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
}

void loop() {
  // Receber dados do script do PC.
  String dadosScript = obterDados();
  if (!dadosScript.empty()) {
    processarDados(dadosScript);
  }

  String dados = "[Cpu Usage] " + cpuUsage + " [Cpu Temp] " + temperatura + " [Mem Usage] " + memUsage;
  publishLog(dados);

  // Verificar conexão com o MQTT Broker
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Publicar dados no MQTT
  publishCpuTemp(temperatura);
  publishCpuUsage(cpuUsage);
  publishMemUsage(memUsage);

  delay(120000);  // Atraso de 2 minutos
}

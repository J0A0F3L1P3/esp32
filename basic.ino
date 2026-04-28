/*
  PROJETO BASE - PAINEL DE CONTROLE ESP32 VIA WIFI
  Uso didático para próximas aulas

  Recursos incluídos:
  - Modo Access Point (ESP32 cria sua própria rede Wi-Fi)
  - Página HTML organizada
  - Botões comentados e identificados
  - Controle de pinos GPIO
  - Status em tempo real
  - Estrutura pronta para expansão
*/

#include <WiFi.h>

// =========================
// CONFIGURAÇÕES DA REDE
// =========================
const char* ssid = "ESP32_PAINEL_AULA";
const char* password = "12345678";

WiFiServer server(80);

// =========================
// PINOS UTILIZADOS
// =========================
const int LED1 = 2;   // LED onboard
const int LED2 = 4;   // saída auxiliar
const int MOTOR = 5;  // exemplo para relé / motor

bool estadoLed1 = false;
bool estadoLed2 = false;
bool estadoMotor = false;

// =========================
// GERA HTML DINÂMICO
// =========================
String gerarPagina() {
  String html = "<!DOCTYPE html>";
  html += "<html lang='pt-BR'>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Painel ESP32</title>";
  html += "<style>";
  html += "body{font-family:Arial;background:#f4f4f4;text-align:center;padding:20px;}";
  html += ".card{background:white;padding:25px;border-radius:12px;max-width:700px;margin:auto;box-shadow:0 4px 10px rgba(0,0,0,0.1);}";
  html += ".btn{display:block;width:100%;padding:15px;margin:10px 0;font-size:18px;border:none;border-radius:8px;cursor:pointer;}";
  html += ".on{background:#28a745;color:white;}";
  html += ".off{background:#dc3545;color:white;}";
  html += ".info{background:#007bff;color:white;}";
  html += "p{text-align:left;line-height:1.6;}";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<div class='card'>";
  html += "<h1>Painel de Controle ESP32</h1>";
  html += "<h3>Base para aulas futuras</h3>";

  // =========================
  // BOTÃO 1 - LED INTERNO
  // =========================
  html += "<a href='/led1'><button class='btn on'>Botão 1 - Alternar LED Interno (GPIO 2)</button></a>";
  html += "<p><b>Função:</b> Liga ou desliga o LED interno da placa. Ideal para ensinar saída digital.</p>";

  // =========================
  // BOTÃO 2 - SAÍDA AUXILIAR
  // =========================
  html += "<a href='/led2'><button class='btn off'>Botão 2 - Alternar Saída Auxiliar (GPIO 4)</button></a>";
  html += "<p><b>Função:</b> Controla um segundo LED, buzzer ou outro componente.</p>";

  // =========================
  // BOTÃO 3 - MOTOR / RELÉ
  // =========================
  html += "<a href='/motor'><button class='btn info'>Botão 3 - Motor / Relé (GPIO 5)</button></a>";
  html += "<p><b>Função:</b> Simula acionamento de motor, relé ou carga externa.</p>";

  // =========================
  // STATUS DOS PINOS
  // =========================
  html += "<hr>";
  html += "<h3>Status Atual</h3>";
  html += "<p>LED 1: " + String(estadoLed1 ? "LIGADO" : "DESLIGADO") + "</p>";
  html += "<p>LED 2: " + String(estadoLed2 ? "LIGADO" : "DESLIGADO") + "</p>";
  html += "<p>Motor: " + String(estadoMotor ? "LIGADO" : "DESLIGADO") + "</p>";

  html += "</div>";
  html += "</body>";
  html += "</html>";

  return html;
}

void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(MOTOR, OUTPUT);

  WiFi.softAP(ssid, password);
  server.begin();

  Serial.println("Servidor iniciado");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  WiFiClient client = server.available();

  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  // =========================
  // CONTROLE DOS BOTÕES
  // =========================
  if (request.indexOf("/led1") != -1) {
    estadoLed1 = !estadoLed1;
    digitalWrite(LED1, estadoLed1);
  }

  if (request.indexOf("/led2") != -1) {
    estadoLed2 = !estadoLed2;
    digitalWrite(LED2, estadoLed2);
  }

  if (request.indexOf("/motor") != -1) {
    estadoMotor = !estadoMotor;
    digitalWrite(MOTOR, estadoMotor);
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(gerarPagina());

  delay(1);
  client.stop();
}

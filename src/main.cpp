#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Stepper.h>
#include <Esteiras.h>

/* Definições para a conexão do esp com a rede WiFi */
#define ENTERPRISE

#ifdef ENTERPRISE
#define EAP_SSID "eduroam"
#define EAP_IDENTITY "a2093561"
#define EAP_USERNAME "a2093561"
#define EAP_PASSWORD "newschneider452"
#else
#define EAP_SSID "CASA"
#define EAP_PASSWORD "SENHA"
#endif

AsyncWebServer server(80);

const char* PARAM_MESSAGE = "message";

/* Definições das esteiras */
Stepper esteira1(passosPorRevolucao, esteira1_IN1, esteira1_IN3, esteira1_IN2, esteira1_IN4);
Stepper esteira2(passosPorRevolucao, esteira2_IN1, esteira2_IN3, esteira2_IN2, esteira2_IN4);
Stepper esteira3(passosPorRevolucao, esteira3_IN1, esteira3_IN3, esteira3_IN2, esteira3_IN4);
Stepper esteira4(passosPorRevolucao, esteira4_IN1, esteira4_IN3, esteira4_IN2, esteira4_IN4);

void configurarEsteiras();

void rotinaEsteira1();
void rotinaEsteira2();
void rotinaEsteira3();

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String message;

String EstadoEsteira1 = "OFF";
String EstadoEsteira2 = "OFF";
String EstadoEsteira3 = "OFF";
String EstadoEsteira4 = "OFF";

void setup() {

  Serial.begin(115200);

#ifdef ENTERPRISE
  WiFi.begin(EAP_SSID, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);
#else
  WiFi.begin(EAP_SSID, EAP_PASSWORD);
#endif

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.print("\nIP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasParam(PARAM_MESSAGE, true)) {
      message = request->getParam(PARAM_MESSAGE, true)->value();
    } else {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Hello, POST: " + message);
  });

  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "E1"+EstadoEsteira1+"|E2"+EstadoEsteira2+"|E3"+EstadoEsteira3+"|E4"+EstadoEsteira4);
  });

  server.onNotFound(notFound);

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Accept, Content-Type, Authorization");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Credentials", "true");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

  server.begin();

  configurarEsteiras();
}

void loop() {
  if(message == "1") {
    rotinaEsteira1();
    message = "";
  }
  if(message == "2") {
    rotinaEsteira2();
    message = "";
  }
  if(message == "3") {
    rotinaEsteira3();
    message = "";
  }
}

void configurarEsteiras() {
  esteira1.setSpeed(6);
  esteira2.setSpeed(6);
  esteira3.setSpeed(6);
  esteira4.setSpeed(25);
}

void rotinaEsteira1() {
  EstadoEsteira1 = "ON";
  esteira1.step(-passosPorRevolucao + 200);
  EstadoEsteira1 = "OFF";
  delay(500);
  EstadoEsteira4 = "ON";
  esteira4.step(passosPorRevolucao*4);
  Serial.println("FIM ESTEIRA 1");
  EstadoEsteira4 = "OFF";
}

void rotinaEsteira2() {
  EstadoEsteira2 = "ON";
  esteira2.step(passosPorRevolucao + 200);
  EstadoEsteira2 = "OFF";
  delay(500);
  EstadoEsteira4 = "ON";
  esteira4.step(passosPorRevolucao*4);
  Serial.println("FIM ESTEIRA 2");
  EstadoEsteira4 = "OFF";
}

void rotinaEsteira3() {
  EstadoEsteira3 = "ON";
  esteira3.step(-passosPorRevolucao + 200);
  EstadoEsteira3 = "OFF";
  delay(500);
  EstadoEsteira4 = "ON";
  esteira4.step(passosPorRevolucao*4);
  Serial.println("FIM ESTEIRA 3");
  EstadoEsteira4 = "OFF";
}
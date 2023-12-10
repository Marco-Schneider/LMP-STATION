// #include <Arduino.h>
// #include <Stepper.h>
// #include <Esteiras.h>
// #include <ArduinoJson.h>
// #include <AsyncTCP.h>
// #include <WiFi.h>
// #include <ESPAsyncWebServer.h>

// void configurarEsteiras();

// const char* ssid = "Claro5G";
// const char* password = "96556798";

// AsyncWebServer server(80);

// void setup() {
//   Serial.begin(115200);

//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);

//   while(WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Connecting to Wi-Fi network...");
//   }
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());

//   server.on("/load", HTTP_POST, [](AsyncWebServerRequest * request) 
//   {
//     int paramsNr = request->params(); // number of params (e.g., 1)
//     Serial.println(paramsNr);
//     Serial.println();
    
//     AsyncWebParameter * j = request->getParam(0); // 1st parameter
//     Serial.print("Size: ");
//     Serial.print(j->value());                     // value ^
//     Serial.println();

//     request->send(200);
//   });

//   server.begin();
// }

// void loop() {

// }

//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <Stepper.h>
#include <Esteiras.h>

AsyncWebServer server(80);

const char* ssid = "Claro5G";
const char* password = "96556798";

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

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasParam(PARAM_MESSAGE, true)) {
      message = request->getParam(PARAM_MESSAGE, true)->value();
    } else {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Hello, POST: " + message);
  });

  server.onNotFound(notFound);

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
  esteira1.step(-passosPorRevolucao*2);
  delay(500);
  esteira4.step(passosPorRevolucao*4);
  Serial.println("FIM ESTEIRA 1");
}

void rotinaEsteira2() {
  esteira2.step(passosPorRevolucao*2);
  delay(500);
  esteira4.step(passosPorRevolucao*4);
  Serial.println("FIM ESTEIRA 2");
}

void rotinaEsteira3() {
  esteira3.step(-passosPorRevolucao*2);
  delay(500);
  esteira4.step(passosPorRevolucao*4);
  Serial.println("FIM ESTEIRA 3");
}
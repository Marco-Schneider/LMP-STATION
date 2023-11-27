#include <Arduino.h>
#include <Stepper.h>
#include <ESP32Servo.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

const int passosPorRevolucao = 2048;

//Esteira 1
const int esteira1_IN1 = 33;
const int esteira1_IN2 = 32;
const int esteira1_IN3 = 21;
const int esteira1_IN4 = 22;

Stepper esteira1(passosPorRevolucao, esteira1_IN1, esteira1_IN3, esteira1_IN2, esteira1_IN4);

//Esteira 2
const int esteira2_IN1 = 18;
const int esteira2_IN2 = 19;
const int esteira2_IN3 = 21;
const int esteira2_IN4 = 22;

Stepper esteira2(passosPorRevolucao, esteira2_IN1, esteira2_IN3, esteira2_IN2, esteira2_IN4);

//Esteira 3
const int esteira3_IN1 = 5;
const int esteira3_IN2 = 17;
const int esteira3_IN3 = 16;
const int esteira3_IN4 = 4;

Stepper esteira3(passosPorRevolucao, esteira3_IN1, esteira3_IN3, esteira3_IN2, esteira3_IN4);

//Esteira 4
const int esteira4_IN1 = 14;
const int esteira4_IN2 = 27;
const int esteira4_IN3 = 26;
const int esteira4_IN4 = 25;

Stepper esteira4(passosPorRevolucao, esteira4_IN1, esteira4_IN3, esteira4_IN2, esteira4_IN4);

void setup() {
  SerialBT.begin("LMP-STATION");
  esteira1.setSpeed(5);
  esteira2.setSpeed(5);
  esteira3.setSpeed(5);
  esteira4.setSpeed(5);
}

void loop() {
  if(SerialBT.available()) {
    String serialInput = SerialBT.readStringUntil('\n');
    serialInput.replace("\r", "");

    if(serialInput == "a") {
      SerialBT.println("Ativando esteira 1");
      esteira1.step(passosPorRevolucao);
      delay(500);
    }
    else if(serialInput == "b") {
      SerialBT.println("Ativando esteira 2");
      esteira2.step(passosPorRevolucao);
      delay(500);
    }
    else if(serialInput == "c") {
      SerialBT.println("Ativando esteira 3");
      esteira3.step(passosPorRevolucao);
      delay(500);
    }
    else if(serialInput == "d") {
      SerialBT.println("Ativando esteira 4");
      esteira4.step(passosPorRevolucao);
      delay(500);
    }
    else {
      SerialBT.println("Comando invalido");
    }

  }
}
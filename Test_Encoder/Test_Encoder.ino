#include <ESP8266WiFi.h>
/* ---------------- MOTORE -------------------- */
// Pin motore pwm
uint8_t enA = 5; 
// Pins motore per rotazione
// in1 = HIGH -> Antiorario, in2 = HIGH -> Orario
uint8_t in1 = 4;
uint8_t in2 = 16;
// Carattersitiche del motore
const float riduzione = 588.0;
const float imp_giro = 11.0;
float pos = 0;
/* -------------------------------------------- */
//////////////////////////////////////////////////
/* ---------------- ENCODER ------------------- */
uint8_t encoderPinA = 2;
uint8_t encoderPinB = 0;  
int countA = 0;
ICACHE_RAM_ATTR void isrA() {
  if(digitalRead(encoderPinB) != digitalRead(encoderPinA)) {
    countA ++;   
  } else {
    countA --;
  }
}
/* -------------------------------------------- */
void setup() {

  Serial.begin(9600);
  // Definizione come outpu dei pin collegati a L296N
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
  
  // Definizione come input con pullup del pin dell'encoder A
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), isrA, RISING);
  
}

void loop() {
  analogWrite(enA, 80);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  pos = 360.0*countA/(imp_giro*riduzione);
  Serial.print("Posizione: ");Serial.println(pos);
  
}

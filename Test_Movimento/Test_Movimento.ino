#include <ESP8266WiFi.h>
/* ---------------- MOTORE ----------------- */
// Pin motore pwm
uint8_t enA = 5; 
// Pins motore per rotazione
// in1 = HIGH -> Antiorario, in2 = HIGH -> Orario
uint8_t in1 = 4;
uint8_t in2 = 16;
/* ------------------------------------------ */

void setup() {
  
  // Definizione come outpu dei pin collegati a L296N
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  analogWrite(enA, 150);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void loop() {
  
  

}

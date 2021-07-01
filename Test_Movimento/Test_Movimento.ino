#include <ESP8266WiFi.h>

/* ---------------- MOTORE ----------------- */
// Pin motore pwm
int enA = 4; 
// Pins motore per rotazione
// in1 = HIGH -> Antiorario, in2 = HIGH -> Orario
int in1 = 0;
int in2 = 2;
int flag = 1;
void avviamentoMotore(){
  if (flag == 1){ 
    analogWrite(enA, 255);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    flag = 0;
  } 
}
/* ------------------------------------------ */

void setup() {
  
  // Definizione come outpu dei pin collegati a L296N
  Serial.begin(9600);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
}

void loop() {
  avviamentoMotore();
  
  

}

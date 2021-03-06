#include <ESP8266WiFi.h>
/* ---------------- MOTORE -------------------- */
// Pin motore pwm
uint8_t enA = 4; 
// Pins motore per rotazione
// in1 = HIGH -> Antiorario, in2 = HIGH -> Orario
uint8_t in1 = 0;
uint8_t in2 = 2;
// Carattersitiche del motore
const float riduzione = 600.0;
const float imp_giro = 11.0;
float pos = 0;
int flag = 1;
void avviamentoMotore(){
  if (flag == 1){ 
    analogWrite(enA, 90);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    flag = 0;
  } 
}
/* -------------------------------------------- */
//////////////////////////////////////////////////
/* ---------------- ENCODER ------------------- */
uint8_t encoderPinA = 14;
uint8_t encoderPinB = 12;  
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
  avviamentoMotore();
  pos = 360.0*countA/(imp_giro*riduzione);
  if (pos >= 360){
    countA = 0;
  }
  Serial.print("Posizione: ");Serial.println(pos);
  
}

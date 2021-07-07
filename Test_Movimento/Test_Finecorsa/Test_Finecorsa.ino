#include <ESP8266WiFi.h>
/* ---------------- MOTORE -------------------- */
// Pin motore pwm
uint8_t enA = 4; 
// Pins motore per rotazione
// in1 = HIGH -> Antiorario, in2 = HIGH -> Orario
uint8_t in1 = 0;
uint8_t in2 = 2;
// Carattersitiche del motore
const float riduzione = 595.0;
const float imp_giro = 11.0;
float pos = 0;
float pos_1 = 0;
float pos_2 = 0;
int giro = 0;
float fc_sx;
float fc_dx;
int flag = 1;
int i = 0;
void avviamentoMotore(){
  if (flag == 1){ 
<<<<<<< Updated upstream
    analogWrite(enA, 250);
=======
    analogWrite(enA, 100 );
>>>>>>> Stashed changes
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
  //Serial.print("Posizione: "); Serial.print(pos); Serial.print(", ");Serial.print("Posizione_1: ");Serial.print(pos_1); Serial.print(", ");Serial.print("Posizione_2: ");Serial.print(pos_2);Serial.print(", ");Serial.print("i: ");Serial.println(i);
  if (abs(pos) >= 360){
    countA = 0;
  } 
  
  if (pos == pos_1){
    i++;
    if (pos_1 != pos_2){
    i = 0;
    }
    if (i == 100000){
      if (giro == 0){
        fc_sx = pos;
        analogWrite(enA, 0);
        Serial.print("fc_sx: ");Serial.println(fc_sx);
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        delay(1000);  
        analogWrite(enA, 220);
        giro = 1;
        i=0;
      } else {
        fc_dx = pos;
        analogWrite(enA, 0);
        Serial.print("fc_dx: ");Serial.println(fc_dx);
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        delay(1000);
        analogWrite(enA, 220);
        i=0;
        giro = 0;
      }    
    }
  }


  pos_2 = pos_1;
  pos_1 = pos;
  
  
}

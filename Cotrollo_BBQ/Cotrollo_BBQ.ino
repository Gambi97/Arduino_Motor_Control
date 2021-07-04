#include <ESP8266WiFi.h>
/* ****************************************************************************************************************** */
/* ---------------- Termocoppia -------------------- */
#include <MAX6675_Thermocouple.h>
#include <Ewma.h>
Ewma Filter(0.05);   // più piccolo più smooth però con più ritardo
#define SCK 15
#define CS 13 //MOSI
#define SO 3
MAX6675_Thermocouple tc = MAX6675_Thermocouple(SCK, CS, SO);
float t = 0; //temperatura
unsigned long previousTime = 0;
unsigned long currentTime = 0;
const unsigned long eventInterval = 500; //2 secondi 

void readTemp() {
  if (currentTime - previousTime >= eventInterval) {
    t = Filter.filter(tc.readCelsius());
    Serial.println(t);
    previousTime = currentTime;
  }
}
/* ********************************************************************************************************************* */
/* ---------------- MOTORE -------------------- */
// Pin motore pwm
uint8_t enA = 4; 
// Pins motore per rotazione
// in1 = HIGH -> Antiorario, in2 = HIGH -> Orario (da verificare)
uint8_t in1 = 0;
uint8_t in2 = 2;
// Carattersitiche del motore
const float riduzione = 600.0;
const float imp_giro = 11.0;
float pos = 0;

/* ********************************************************************************************************************* */
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
void checkFinecorsa(){
  
}
/* ********************************************************************************************************************* */
void setup() {
  Serial.begin(9600);
  // Definizione come outpu dei pin collegati a L296N
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(SO, INPUT);
  pinMode(CS, OUTPUT);
  pinMode(SCK, OUTPUT);
      
  // Definizione come input con pullup del pin dell'encoder A
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), isrA, RISING);

  
}

void loop() {
  // Misuro temperatura del bbq ogni 5s 
  currentTime = millis();
  readTemp();

  

}

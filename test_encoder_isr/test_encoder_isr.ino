#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
BlynkTimer timer;
//defenizione wifi e blynk
char auth[] = "iGWUmQ9TEi-0bPVPxMf76sfcHKNkTC4X";

char ssid[] = "";
char pass[] = "";


// MOTORE
int ENB = 4; // Pin motore pwm
int IN3 = 0;
int IN4 = 2;

// ENCODER
int encoderPinA = 14; 
int encoderPinB = 12; 
int countA = 0;
int countB = 0;
int protectedCount = 0;
int previousCount = 0;

// Controllore 
const float Kp = 50;
const float Ti = 0.1;
const float Td = 0;
const float dT = 30e-3;
float e = 0;
float e_1 = 0; //Errore istante precedente
float e_2 = 0; //Errore 2 istanti precedenti
const int u_max = 255; 
const int u_min = 60; // altrimenti motore non si muove 

// VARIABILI 
int stato = HIGH;
BLYNK_WRITE(V4)    
{
  stato = param.asInt();
}
unsigned long myTime;
float u = 0;
int riferimento = 0;
BLYNK_WRITE(V1)    
{
  riferimento = param.asInt();
}
const float riduzione = 588.0;
const float imp_giro = 11.0;
int flag = 1;
float pos = 0;
void sendvalue(){
  Blynk.virtualWrite(V2, pos); 
  Blynk.virtualWrite(V3, riferimento); 
}


void setup() {

  // Blink setup
  Blynk.begin(auth,ssid,pass);
  Serial.begin(9600);
  timer.setInterval(800L, sendvalue);
  // set all the motor control pins to outputs
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  // PIN encoder
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), isrA, RISING);
  //attachInterrupt(digitalPinToInterrupt(encoderPinB), isrB, CHANGE);
}

void loop() {
  noInterrupts();
  Blynk.run();
  timer.run(); 
  if (stato == HIGH){ 
    avviamentoMotore();
    interrupts();
    pos = 360.0*countA/(imp_giro*riduzione);
    computeControlAction();
  }else{
    spegnimentoMotore();
  }
  /*
  //monitor
  Serial.print("Posizione: ");
  Serial.print(pos);
  Serial.print(", Azione Controllo: ");
  Serial.print(u);
  Serial.print(", Errore: ");
  Serial.println(e);
  */
  // plotter
  Serial.print("Posizione: ");Serial.print(pos);Serial.print(" ");
  Serial.print("Riferimento: ");Serial.print(riferimento);Serial.print(" ");
  Serial.print("Azione Controllo: ");Serial.print(u);Serial.println();
  
  /*
  if ( millis() > 15000){
    riferimento = 359;
  }
  if (millis() > 25000){
    riferimento = 220;
  }
  */
  
  }
  
  
  
ICACHE_RAM_ATTR void isrA() {
  if(digitalRead(encoderPinB) != digitalRead(encoderPinA)) {
    countA ++;
    
  } else {
    countA --;
  }
}
/*
ICACHE_RAM_ATTR void isrB() {
  if (digitalRead(encoderPinB) == digitalRead(encoderPinA)) {
    countB ++;
  } else {
    countB --;
  }
}
*/
void avviamentoMotore(){
  if (flag == 1){ 
    analogWrite(ENB, 250);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    flag = 0;
  } 
}
void computeControlAction(){
  /*Serial.print("Time:= ");
  myTime = millis();
  Serial.println(myTime);*/
  float K1 = Kp*(1 + dT/Ti + Td/dT);
  float K2 = -Kp*(1 + 2*Td/dT); 
  float K3 = Kp*Td/dT;
  
  e = riferimento - pos;
  if ( e < 0){
    e = abs(e);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }else{
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW); 
    
  }
  u = K1*e + K2*e_1 + K3*e_2;
  /*if (u < 0){
    u = 0;
  }*/
  if (abs(u) < u_min){
    u = u_min;
  }
  if (abs(u) > u_max){
    u = u_max;
  }
  
  // Spengo i motori se errore minore di 1
  if (abs(e) > 1){
    analogWrite(ENB, u);
  }else{
    u = 0;
    analogWrite(ENB,u);  
  }
  
  e_2 = e_1;
  e_1 = e;
}
void spegnimentoMotore(){
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, 0);
    
}

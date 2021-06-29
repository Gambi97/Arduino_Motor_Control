//#include <SPI.h>
//#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h>
#include <MAX6675_Thermocouple.h>

#define SCK 15
#define CS 13
#define SO 3
MAX6675_Thermocouple tc = MAX6675_Thermocouple(SCK, CS, SO);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp = tc.readCelsius();
  Serial.println(temp);
}

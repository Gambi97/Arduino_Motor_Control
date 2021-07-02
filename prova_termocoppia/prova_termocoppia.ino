#include <SPI.h>
//#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h>
#include <MAX6675_Thermocouple.h>
#include <Ewma.h>
Ewma adcFilter1(0.05);   // Less smoothing - faster to detect changes, but more prone to noise
Ewma adcFilter2(0.01);  // More smoothing - less prone to noise, but slower to detect changes

#define SCK 15
#define CS 13 //MOSI
#define SO 9
MAX6675_Thermocouple tc = MAX6675_Thermocouple(SCK, CS, SO);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp_g = (tc.readCelsius());
  float temp = adcFilter1.filter(tc.readCelsius());
  float temp_smooth = adcFilter2.filter(tc.readCelsius());
  Serial.print(temp);Serial.print(" ");Serial.print(temp_smooth);Serial.print(" ");Serial.println(temp_g);
  delay(3000);
}

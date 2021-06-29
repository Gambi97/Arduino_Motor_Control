#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//defenizione wifi e blynk
char auth[] = "iGWUmQ9TEi-0bPVPxMf76sfcHKNkTC4X";
//char ssid[] = "G-SKY";
//char pass[] = "Famiglia1Gambera";

// MOTORE
int ENB = 4; // Pin motore pwm
int IN3 = 0;
int IN4 = 2;

// ENCODER
int encoderA = 14;
int encoderB = 12;
int valore = 0;
int valore_1 = 10;

void setup() {
  // set all the motor control pins to outputs
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  // PIN encoder
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  //Setup blynk
  Serial.begin(9600);
  //Blynk.begin(auth,ssid,pass);
}  

int letturaPrecedente = HIGH;
int flag = 1;
float pos;
int cont;
// this function will run the motors in both directions at a fixed speed
void loop() {
  //Blynk.run();

  //Avvio il motore
  avviamentoMotore();
  letturaEncoder();
  Serial.println("a");
  delay(10000);
  if ( valore == valore_1){
    spegnimentoMotore();
  }
  valore_1 = valore;
  delay(1);
}

void avviamentoMotore(){
  if (flag == 1){ 
    analogWrite(ENB, 250);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    flag = 0;
  } 
}
void letturaEncoder(){
  int n = digitalRead(encoderA);
  if ((letturaPrecedente == HIGH) && (n == LOW)){
    if (digitalRead(encoderB)) {
      valore++;
    } else {
      valore--;     
    }    
  }
  letturaPrecedente = n;
  
  }

void spegnimentoMotore(){
    analogWrite(ENB, 0);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}
/*
BLYNK_WRITE(V1) {
  int n = param.asInt();
  Serial.println(n);
  analogWrite(ENB, n);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  
}
*/

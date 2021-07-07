struct finecorsa
{
  float sx;
  float dx;
};
struct finecorsa fc = {0,90};
// Pin motore pwm
uint8_t ENB = 4; 
// Pins motore per rotazione
// in1 = HIGH -> Antiorario, in2 = HIGH -> Orario
uint8_t IN3 = 2;
uint8_t IN4 = 0;
// Carattersitiche del motore
const float riduzione = 600.0;
const float imp_giro = 11.0;
float pos = 0;
float pos_norm = 0;
int flag = 1;
void avviamentoMotore(){
  if (flag == 1){ 
    analogWrite(ENB, 90);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
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
// Controllore 
const float Kp = 5;
const float Ti = 0.065 ;
const float Td = 0;
float dT = 30e-3;
float e = 0;
float e_1 = 0; //Errore istante precedente
float e_2 = 0; //Errore 2 istanti precedenti
const int u_max = 255; 
const int u_min = 40; // altrimenti motore non si muove 
float u = 0;
float riferimento = 90;
float riferimento_norm = 0;
void computeControlAction(){
  
  float K1 = Kp*(1 + dT/Ti + Td/dT);
  float K2 = -Kp*(1 + 2*Td/dT); 
  float K3 = Kp*Td/dT;
  if (fc.sx > fc.dx){
    pos_norm = map(pos, fc.dx, fc.sx, -100,100); // normalizzo tra -100 e 100 (full close / full open) //il nuovo range è int, 201 valori 
    //riferimento_norm = constrain(riferimento_norm, fc.dx, fc.sx);  // Controllo extra per non andare fuori range (non dovrebbe servire)
  }
  else if (fc.sx < fc.dx) {
    pos_norm = map(pos, fc.sx, fc.dx, -100,100);
    //riferimento_norm = constrain(riferimento_norm, fc.sx, fc.dx);
  }
  e = riferimento_norm - pos_norm;
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






/*
struct finecorsa checkFinecorsa(float pos, float pos_1, float pos_2) {
    struct finecorsa finecorsa;

    finecorsa.sx = ....
    finecorsa.dx = ....

    return finecorsa;
}
*/
void setup() {
  Serial.begin(9600);
    // Definizione come outpu dei pin collegati a L296N
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  
  // Definizione come input con pullup del pin dell'encoder A
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), isrA, RISING);
  
  
}

void loop() {
  // put your setup code here, to run once:
  

  if (fc.sx > fc.dx){
    riferimento = constrain(riferimento, fc.dx, fc.sx);  
    riferimento_norm = map(riferimento, fc.dx, fc.sx, -100,100); // normalizzo tra -100 e 100 (full close / full open) //il nuovo range è int, 201 valori 
    
  }
  else if (fc.sx < fc.dx) {
    riferimento = constrain(riferimento, fc.sx, fc.dx);
    riferimento_norm = map(riferimento, fc.sx, fc.dx, -100,100);
    
  }

  avviamentoMotore();
  pos = 360.0*countA/(imp_giro*riduzione);
  if (pos >= 360){
    countA = 0;
  }
  Serial.print(pos_norm);Serial.print(",");Serial.print(riferimento_norm);Serial.print(",");Serial.println(u);
  computeControlAction();
  if ( millis() > 5000){
    riferimento = 20;
  }
  if (millis() > 10000){
    riferimento = 220;
  }
  /*
  //struct finecorsa finecorsa;
  //finecorsa = checkFinecorsa(pos,pos_1,pos_2);  // non posso conoscere a prescindere il segno del fine corsa
  // in questo modo controllo il segno
  if (finecorsa.sx > finecorsa.dx){
    riferimento_normalizzato = map(riferimento, finecorsa.dx, finecorsa.sx, -100,100) // normalizzo tra -100 e 100 (full close / full open) //il nuovo range è int, 201 valori 
    riferimento_normalizzato = constrain(riferimento, finecorsa.dx, finecorsa.sx)  // Controllo extra per non andare fuori range (non dovrebbe servire)
  }
  else if (finecorsa.sx < finecorsa.dx) {
    riferimento_normalizzato = map(riferimento, finecorsa.sx, finecorsa.dx, -100,100)
    riferimento_normalizzato = constrain(riferimento, finecorsa.sx, finecorsa.dx)
  }
  */

}

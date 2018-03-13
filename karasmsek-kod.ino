#include <QTRSensors.h>
#include <SharpDistSensor.h>

//color
#define S0 42
#define S1 43
#define S2 44
#define S3 45
#define sensorOut 3

int color;
int greenFrequency = 0;


//QTR RC
#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

QTRSensorsRC qtrrc((unsigned char[]) {30, 31, 32, 33, 34, 35, 36, 37}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];

int beyazcizgi = 0;

int position = 0;

int tercih;

//motors
int eA = 12;                //motor A pow00000000000
int pA1 = 11;               //motor A direction 1 pin
int pA2 = 10;               //motor A direction 2 pin
int eB = 7;                 //motor B power pin
int pB1 = 6;                //motor B direction 1 pin
int pB2 = 5;                //motor B direction 2 pin

//sharps
const byte sensorPin0 = A0;
const byte sensorPin1 = A1;
const byte sensorPin2 = A2;
const byte sensorPin3 = A3;
const byte sensorPin4 = A4;

const byte mediumFilterWindowSize = 3;

SharpDistSensor sensor1(sensorPin0, mediumFilterWindowSize);
SharpDistSensor sensor2(sensorPin1, mediumFilterWindowSize);
SharpDistSensor sensor3(sensorPin2, mediumFilterWindowSize);
SharpDistSensor sensor4(sensorPin3, mediumFilterWindowSize);
SharpDistSensor sensor5(sensorPin4, mediumFilterWindowSize);


void setup()
{
  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    
  for (int i = 0; i < 400; i++){
    qtrrc.calibrate();       
  }
  digitalWrite(13, LOW);    

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(sensorOut, INPUT);

  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++){
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  for (int i = 0; i < NUM_SENSORS; i++){
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);

  pinMode(eA, OUTPUT);
  pinMode(pA1, OUTPUT);
  pinMode(pA2, OUTPUT);
  pinMode(eB, OUTPUT);
  pinMode(pB1, OUTPUT);pinMode(pB2, OUTPUT);
} 



void loop(){
unsigned int distance = 252;/*sensor.getDist();*/
while(distance > 250){
   //Get distance from sensor
  distance = sensor3.getDist();

  
  delay(50);
  position = qtrrc.readLine(sensorValues);
  
  for (unsigned char i = 0; i < NUM_SENSORS; i++){
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }        
 
  Serial.println(position); // comment this line out if you are using raw values
  delay(250);
  cizgitakip();
  }
}


void cizgitakip(){
  if(position <= 3100  && position >= 3900){
    analogWrite(eA, 128);
    analogWrite(eB, 128);
    }
  else if(position > 3900){
    analogWrite(eA, 128);
    analogWrite(eB, 64);
    }
  else{
    analogWrite(eA, 64);
    analogWrite(eB, 128);
    }
  }


void forward(){
  digitalWrite(pA1, HIGH);
  digitalWrite(pA2, LOW);
  digitalWrite(pB1, HIGH);
  digitalWrite(pB2, LOW);
}

void backward(){
  digitalWrite(pA1, LOW);
  digitalWrite(pA2, HIGH);
  digitalWrite(pB1, LOW);
  digitalWrite(pB2, HIGH);
}

void left(int dsure){
  digitalWrite(pA1, HIGH);
  digitalWrite(pA2, LOW);
  digitalWrite(pB1, LOW);
  digitalWrite(pB2, HIGH);
  analogWrite(eA, 50);
  analogWrite(eB, 50);
  delay(dsure);
}

void right(int dsure){
  digitalWrite(pB1, HIGH);
  digitalWrite(pB2, LOW);
  digitalWrite(pA1, LOW);
  digitalWrite(pA2, HIGH);
  analogWrite(eA, 50);
  analogWrite(eB, 50);
  delay(dsure);
}

void coast(){
  digitalWrite(eA, LOW);
  digitalWrite(eB, LOW);
}

void breaks(){
  digitalWrite(eA, HIGH);
  digitalWrite(eB, HIGH);
}

void asama2() {

bool sagserit = false;
bool solserit = false;

int seritsayaci = 0;

while(position != 7000){


if(sensor2.getDist() > 400 && seritsayaci <= 2){      // sharp'lar için girile sınır değerler rastgeledir şu anda.
  solserit = true;
  }
else{
  solserit = false;
  }


if(sensor4.getDist() > 400 && seritsayaci >= -2){
  sagserit = true;
  }
else{
  solserit = false;
  }


switch (solserit){
  
  case true:
  if(seritsayaci > 0){
  left(400);
  while(sensorValues[7] >> 700){
    analogWrite(eA, 50);
    analogWrite(eB, 50);
    }  
  seritsayaci++;
  }
  
  break;
  
  default:
  break;
  }            


switch (sagserit){
  
  case true:
  if(seritsayaci < 0){
    left(400);
  while(sensorValues[7] >> 700){
    analogWrite(eA, 50);
    analogWrite(eB, 50);
    }  
  seritsayaci--;
    }
  break;
  
  default:
  break;
  }
 
 }
}


void asama1(){

while (position != 7000){
  cizgitakip();
}  

left(400);

while (position != 7000){
  cizgitakip();
}  
analogWrite(eA, 150);
analogWrite(eB, 150);
delay(200);

renkoku();

if(color == 2){
while (position != 7000){
  cizgitakip();
  }  
while (position != 7000){
  cizgitakip();
  }  
}
else{
  while(beyazcizgi != 2){
  analogWrite(eA, 150);
  analogWrite(eB, 150);
  
  if(position == 7000){
  beyazcizgi++;
  }
  }
}

right(200);




analogWrite(eA, 150);
analogWrite(eB, 150);
delay(50);

renkoku();

if(color == 2){
  while (position != 7000){
  cizgitakip();
  }  
  while (position != 7000){
  cizgitakip();
  }  
}
else{
  beyazcizgi = 0;
  while(beyazcizgi != 2){
  analogWrite(eA, 150);
  analogWrite(eB, 150);
  
  if(position == 7000){
  beyazcizgi++;
  }
  }
}

right(400);

beyazcizgi = 0;

while (position != 7000){
  cizgitakip();
}  
while (position != 7000){
  cizgitakip();
}  
while (position != 7000){
  cizgitakip();
}  

}

void asama3() {
tercih = millis() % 2;

while(position != 7000){
  cizgitakip();
  }

if(tercih == 0){
  right(200);
  }

else{
 left(200);
  }


while(position != 7000){
  cizgitakip();
  }

analogWrite(eA, 150);
analogWrite(eB, 150);
delay(1000);

}
void renkoku(){
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  greenFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the GREEN (G) value
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  delay(100);
  }

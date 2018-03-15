#include <QTRSensors.h>
#include <SharpDistSensor.h>

//color
#define S0 42
#define S1 43
#define S2 44
#define S3 45
#define sensorOut 3

int color = 0;  //1 = kırmızı, 2 = sarı, 3 = yeşil
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

int siyahsayac = 0;

//QTR RC
#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

QTRSensorsRC qtrrc((unsigned char[]) {30, 31, 32, 33, 34, 35, 36, 37}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];

int position = 0;
int beyazcizgi = 0;
int tercih;
int distance = 0;

//motors
int eA = 12;                //motor A power pin
int pA1 = 10;               //motor A direction 1 pin
int pA2 = 11;               //motor A direction 2 pin
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

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  pinMode(eA, OUTPUT);
  pinMode(pA1, OUTPUT);
  pinMode(pA2, OUTPUT);
  pinMode(eB, OUTPUT);
  pinMode(pB1, OUTPUT);
  pinMode(pB2, OUTPUT);

  Serial.begin(9600);
} 



void loop(){
forward();
if(beyazcizgi == 0){
  analogWrite(eA, 120);
  analogWrite(eB, 120);
  delay(50);
  beyazcizgi++;
}
cizgitakip();
}


void cizgitakip(){
//  siyahsayac = 0;
//  for(int i = 0; i <= 7; i++){
//    if(sensorValues[i] == 1000){
//      siyahsayac++;
//      }
//    }
  forward();
  position = qtrrc.readLine(sensorValues); 
    for (unsigned char i = 0; i < NUM_SENSORS; i++){
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }        
 
  Serial.print(position); // comment this line out if you are using raw values
  delay(250);
  Serial.println('\t');
  
  if(position <= 4000  && position >= 3000){
    analogWrite(eA, 30);
    analogWrite(eB, 30);
    }
  else if(position > 4000){
    analogWrite(eA, 40);
    analogWrite(eB, 30);
    }
  else{
    analogWrite(eA, 30);
    analogWrite(eB, 40);
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
  digitalWrite(pB1, HIGH);
  digitalWrite(pB2, LOW);
  digitalWrite(pA1, LOW);
  digitalWrite(pA2, HIGH);
  analogWrite(eA, 50);
  analogWrite(eB, 50);
  delay(dsure);
  forward();
}

void right(int dsure){
  digitalWrite(pA1, HIGH);
  digitalWrite(pA2, LOW);
  digitalWrite(pB1, LOW);
  digitalWrite(pB2, HIGH);
  analogWrite(eA, 50);
  analogWrite(eB, 50);
  delay(dsure);
  forward();
}

void coast(){
  digitalWrite(eA, LOW);
  digitalWrite(eB, LOW);
}

void breaks(){
  digitalWrite(pA1, HIGH);
  digitalWrite(pA2, HIGH);
  digitalWrite(pB1, HIGH);
  digitalWrite(pB2, HIGH);
}

void asama2() {

bool sagserit = false;
bool solserit = false;

int seritsayaci = 0;

while(siyahsayac != 8){


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
forward();
while (1){
  cizgitakip();
}  

analogWrite(eA, 50);
analogWrite(eB, 50);
delay(200);

left(400);

while (siyahsayac != 8){
  cizgitakip();
}  
analogWrite(eA, 50);
analogWrite(eB, 50);
delay(200);

renkoku();

if(color == 3){
while (siyahsayac != 8){
  cizgitakip();
  }  
while (siyahsayac != 8){
  cizgitakip();
  }  
}
else{
  while(beyazcizgi != 2){
  backward();
  analogWrite(eA, 70);
  analogWrite(eB, 70);
  
  if(position == 7000){
  beyazcizgi++;
    }
   }
  right(200);



forward();
analogWrite(eA, 150);
analogWrite(eB, 150);
delay(50);

renkoku();

if(color == 3){
  while (siyahsayac != 8){
  cizgitakip();
  }  
  while (siyahsayac != 8){
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
 right(400);


while (siyahsayac != 8){
  cizgitakip();
}  
while (siyahsayac != 8){
  cizgitakip();
}  
while (siyahsayac != 8){
  cizgitakip();
}  
}
}





}

void asama3() {
tercih = millis() % 2;

while(siyahsayac != 8){
  cizgitakip();
  }

if(tercih == 0){
  right(200);
  }

else{
 left(200);
  }


while(siyahsayac != 8){
  cizgitakip();
  }

analogWrite(eA, 150);
analogWrite(eB, 150);
delay(1000);

}
int renkoku(){
  // Setting RED (R) filtered photodiodes to be read
digitalWrite(S2,LOW);
digitalWrite(S3,LOW);

// Reading the output frequency
redFrequency = pulseIn(sensorOut, LOW);

// Printing the RED (R) value
Serial.print("R = ");
Serial.print(redFrequency);
delay(100);

// Setting GREEN (G) filtered photodiodes to be read
digitalWrite(S2,HIGH);
digitalWrite(S3,HIGH);

// Reading the output frequency
greenFrequency = pulseIn(sensorOut, LOW);

// Printing the GREEN (G) value
Serial.print(" G = ");
Serial.print(greenFrequency);
delay(100);


if(20 <= redFrequency && redFrequency <= 30 && 80 <= greenFrequency && greenFrequency <= 100){
  color = 1;
  Serial.println("RED Detected!   ");
  }
else if(15 <= redFrequency && redFrequency <= 25 && 25 <= greenFrequency && greenFrequency <= 35){
  color = 2;
  Serial.println("YELLOW Detected!   ");
  }
else{
   color = 3;
  Serial.println("GREEN Detected!   ");
  }
  Serial.print(color);
return color;
}

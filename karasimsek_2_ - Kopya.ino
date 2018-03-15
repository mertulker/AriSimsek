//bismillahirrahmanirrahim
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
int serit = 0;
int siyahsayac = 0;

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
analogWrite(eA,60);
analogWrite(eB,60);
//if(beyazcizgi == 0){
//  analogWrite(eA, 60);
//  analogWrite(eB, 60);
//  delay(30);
//  beyazcizgi++;
//}
//cizgitakip();
//asama2();

}


void cizgitakip(){
//  siyahsayac = 0;
//  for(int i = 0; i <= 7; i++){
//    if(sensorValues[i] == 1000){
//      siyahsayac++;
//      }
//    }
//  if(siyahsayac >= 3){
//    if(position > 3500){
//      analogWrite(eB, 45);
//      delay(500);
//    }
//    else{
//      analogWrite(eA, 45);
//      delay(500);
//    }
//  }
  forward();
  position = qtrrc.readLine(sensorValues); 
    for (unsigned char i = 0; i < NUM_SENSORS; i++){
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }        
 
  Serial.print(position); // comment this line out if you are using raw values
  delay(250);
  Serial.println('\t');
  
//  if(position <=   && position >= 1500){
//    analogWrite(eA, 50);
//    analogWrite(eB, 50);
//    }
   if(5000<= position && position <=7000){
    analogWrite(eA, 70);
    analogWrite(eB, 50);
    }
   if(3500<= position && position <5000)
   {
    analogWrite(eA, 60);
    analogWrite(eB, 50);
    }
    if(2000<= position && position <3500)
   {
    analogWrite(eA, 50);
    analogWrite(eB, 60);
    }
    if(0<= position&& position <2000)
   {
    analogWrite(eA, 50);
    analogWrite(eB, 70);
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

int asama2(){
  while(true){
    if(sensor3.getDist() < 900){
      if(sensor2.getDist() > 900 && serit > -2){
        left(450);
        while(sensorValues[0] < 700){
          analogWrite(eA, 45);
          analogWrite(eB, 45);
        }
        cizgitakip();
        serit--;
        asama2();
      }
     else if(sensor4.getDist() > 900 && serit < 2){
        right(450);
        while(sensorValues[8] < 700){
          analogWrite(eA, 45);
          analogWrite(eB, 45);
        }
        cizgitakip();
        serit++;
        asama2();
      }
    }
    else { 
      cizgitakip();
    }
  }
  return 0;
}

int asama2ending(){
  int degistirici = 0;
  if(serit < 0){
    backward();
    analogWrite(eB, 45);
    delay(250);
    degistirici = 1; 
  }
  else if(serit > 0){
    backward();
    analogWrite(eA, 45);
    delay(250);
    degistirici = (-1);
  }
  else{
    return 0;
  }
  
  while(serit != 0){
      analogWrite(eA, 35);
      analogWrite(eB, 35);
      siyahsayac = 0;
      for(int i = 0; i <= 7; i++){
        if(sensorValues[i] == 1000){
          siyahsayac++;
         }
       }
       if(siyahsayac == 8){
        serit += degistirici;
       }
        
  }
  if(degistirici < 0){
    left(250);
    return 0;
  }
  else{
    right(250);
    return 0;
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

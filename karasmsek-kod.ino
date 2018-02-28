#include <QTRSensors.h>
#include <SharpDistSensor.h>

#define NUM_SENSORS   8     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

int enableA = 11;
int pinA1 = 10;
int pinA2 = 9;
int enableB = 7;
int pinB1 = 6;
int pinB2 = 5;

//Define Run variable
boolean run;


// Analog pin to which the sensor is connected
const byte sensorPin = A0;

// Window size of the median filter (odd number, 1 = no filtering)
const byte mediumFilterWindowSize = 3;

// Create an object instance of the SharpDistSensor class
SharpDistSensor sensor(sensorPin, mediumFilterWindowSize);

QTRSensorsRC qtrrc((unsigned char[]) {30, 31, 32, 33, 34, 35, 36, 37},
NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];






void setup()
{
delay(500);
pinMode(13, OUTPUT);
digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
{
qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
}
digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration
// print the calibration minimum values measured when emitters were on
Serial.begin(9600);
for (int i = 0; i < NUM_SENSORS; i++)
{
Serial.print(qtrrc.calibratedMinimumOn[i]);
Serial.print(' ');
}
Serial.println();
// print the calibration maximum values measured when emitters were on
for (int i = 0; i < NUM_SENSORS; i++)
{
Serial.print(qtrrc.calibratedMaximumOn[i]);
Serial.print(' ');
}
Serial.println();
Serial.println();
delay(1000);

 pinMode(enableA, OUTPUT);
 pinMode(pinA1, OUTPUT);
 pinMode(pinA2, OUTPUT);
 pinMode(enableB, OUTPUT);
 pinMode(pinB1, OUTPUT);
 pinMode(pinB2, OUTPUT);
 run = true;
} 



void loop(){
digitalWrite(enableA, HIGH);
digitalWrite(enableB, HIGH);
unsigned int distance = sensor.getDist();
while(distance > 250){
// Get distance from sensor
  distance = sensor.getDist();

  // Print distance to Serial
  Serial.print(distance);
  Serial.print("   ");
  // Wait some time
  delay(50);
unsigned int position = qtrrc.readLine(sensorValues);
// print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
// 1000 means minimum reflectance, followed by the line position
for (unsigned char i = 0; i < NUM_SENSORS; i++)
{
Serial.print(sensorValues[i]);
Serial.print('\t');
}        
//Serial.println(); // uncomment this line if you are using raw values
Serial.println(position); // comment this line out if you are using raw values
delay(250);



  if(position <= 3900  && position >= 3100){
    motorAForward();
    motorBForward();
    //delay(3000);
    }
  else if(position > 3900){
    motorAForward();
    motorBCoast();
    //delay(3000);
    }
  else{
    motorACoast();
    motorBForward();
    //delay(3000);
    }
}
}




//Define Low Level H-Bridge Commands
//enable motors
void motorAOn()
{
 digitalWrite(enableA, HIGH);
}
void motorBOn()
{
 digitalWrite(enableB, HIGH);
}
//disable motors
void motorAOff()
{
  digitalWrite(enableB, LOW);
}
void motorBOff()
{
 digitalWrite(enableA, LOW);
}
//motor A controls
void motorAForward()
{
 analogWrite(pinA1, HIGH);
 digitalWrite(pinA2, LOW);
}
void motorABackward()
{
 digitalWrite(pinA1, LOW);
 digitalWrite(pinA2, HIGH);
}
//motor B contorls
void motorBForward()
{
 digitalWrite(pinB1, HIGH);
 digitalWrite(pinB2, LOW);
}
void motorBBackward()
{
 digitalWrite(pinB1, LOW);
 digitalWrite(pinB2, HIGH);
}
//coasting and braking
void motorACoast()
{
 digitalWrite(pinA1, LOW);
 digitalWrite(pinA2, LOW);
}
void motorABrake()
{
 digitalWrite(pinA1, HIGH);
 digitalWrite(pinA2, HIGH);
}
void motorBCoast()
{
 digitalWrite(pinB1, LOW);
 digitalWrite(pinB2, LOW);
 }

void motorBBrake()
{
 digitalWrite(pinB1, HIGH);
 digitalWrite(pinB2, HIGH);
}
//Define High Level Commands
void enableMotors()
{
 motorAOn();
 motorBOn();
}
void disableMotors()
{
 motorAOff();
 motorBOff();
}
void forward(int time)
{
 motorAForward();
 motorBForward();
 delay(time);
}
void backward(int time)
{
 motorABackward();
 motorBBackward();
 delay(time);
}
void turnLeft(int time)
{
 motorABackward();
 motorBForward();
 delay(time);
}
void turnRight(int time)
{
 motorAForward();
 motorBBackward();
 delay(time);
}
void coast(int time)
{
 motorACoast();
 motorBCoast();
 delay(time);
}
void brake(int time)
{
 motorABrake();
 motorBBrake();
 delay(time);
}


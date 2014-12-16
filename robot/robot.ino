/*

 */
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#define maxSpeed 255

#define minServo 10
#define maxServo 169
#define minServoSafe 85
#define maxServoSafe 95

#define ch1pin 2
#define ch2pin 3
#define ch3pin 4
#define ch4pin 5
#define ch5pin 6
#define ch6pin 7
#define servo1pin 9
#define servo2pin 10

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *motor1 = AFMS.getMotor(1);

Servo servo1;
Servo servo2;


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600); 
  
  AFMS.begin();
  motor1->run(RELEASE);
  
  // initialize digital pin 13 as an output.
  pinMode(ch1pin,INPUT);
  pinMode(ch2pin,INPUT);
  pinMode(ch3pin,INPUT);
  pinMode(ch4pin,INPUT);
  pinMode(ch5pin,INPUT);
  pinMode(ch6pin,INPUT);

  servo1.attach(servo2pin);
  //servo2.attach(servo2pin);
  
}

int ch1;
float ch1x;
int ch2;
int ch3;
int ch4;
int ch5;
int ch6;

int ch1val;
int ch2val;
int ch3val;
int ch4val;
int ch5val;
int ch6val;

int way = 1;

// the loop function runs over and over again forever
void loop() {
  ch1 = pulseIn(ch1pin,HIGH,25000);
  //ch2 = pulseIn(ch2pin,HIGH,25000);
  ch3 = pulseIn(ch3pin,HIGH,25000);
  ch4 = pulseIn(ch4pin,HIGH,25000);
  ch5 = pulseIn(ch5pin,HIGH,25000);
  //ch6 = pulseIn(ch6pin,HIGH,25000);

  ch1val = map(ch1,1100,1930,minServo,maxServo);
  if (ch1val < minServo) ch1val = minServo;
  if (ch1val > maxServo) ch1val = maxServo;
  //if (ch1val > minServoSafe && ch1val < maxServoSafe) ch1val = 90;
  
//  ch2val = map(ch2,1100,1920,0,180);
//  if (ch2val < minServo) ch2val = minServo;
//  if (ch2val > maxServo) ch2val = maxServo;
//  if (ch2val > minServoSafe && ch2val < maxServoSafe) ch2val = 90;

  ch3val = map(ch3,1100,1920,0,maxSpeed);
  if (ch3val < 0) ch3val = 0;
  if (ch3val > 255) ch3val = 255;

  ch4val = map(ch4,1100,1920,-90,90);
  if (ch4val < -90) ch4val = -90;
  if (ch4val >  90) ch4val =  90;

  ch5val = map(ch5,900,2100,-1,1);
 
//  ch6val = map(ch6,1100,1920,0,255);
//  if (ch6val < 0) ch6val = 0;
//  if (ch6val > 255) ch6val = 255;

//  Serial.print("ch1 ");
//  Serial.print(ch1val);Serial.print(" ");
  Serial.print(ch1);
  Serial.print(" ");
//  Serial.print("ch2 ");
//  Serial.print(ch2val);Serial.print(" ");
  Serial.print(ch2);
  Serial.print(" ");
//  Serial.print("ch3 ");
//  Serial.print(ch3val);Serial.print(" ");
  Serial.print(ch3);
  Serial.print(" ");
//  Serial.print("ch4 ");
//  Serial.print(ch4val);Serial.print(" ");
  Serial.print(ch4);
  Serial.print(" ");
//  Serial.print("ch5 ");
//  Serial.print(ch5val);Serial.print(" ");
  Serial.print(ch5);
  Serial.print(" ");
//  Serial.print("ch6 ");
//  Serial.print(ch6val);Serial.print(" ");
  Serial.println(ch6);
  
  if (ch3val > 0) {
    if (way < 0) {
      motor1->run(BACKWARD);
    } else {
      motor1->run(FORWARD);
    }
    motor1->setSpeed(ch3val);
  } else {
    motor1->run(RELEASE);
    way = ch5val;
  }
  
  servo1.write(ch1val);
  //servo2.write(ch2val);
  delay(15);
}



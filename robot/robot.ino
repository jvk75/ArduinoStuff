/*

 */
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#define maxSpeed 255

#define minServo1 10
#define maxServo1 169
#define minServo2 30
#define maxServo2 169
#define servo1Home 90
#define servo2Home 90

#define ch1pin 2 
#define ch2pin 3
#define ch3pin 4
#define ch4pin 5
#define ch5pin 6
#define ch6pin 7
#define servo1pin 9
#define servo2pin 10
#define lightsPin 11

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *motor4 = AFMS.getMotor(4);

Servo servo1;
Servo servo2;


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600); 
  
  AFMS.begin();
  motor1->run(RELEASE);
  motor2->run(RELEASE);
  motor3->run(RELEASE);
  motor4->run(RELEASE);
  
  // initialize digital pin 13 as an output.
  pinMode(ch1pin,INPUT);
  pinMode(ch2pin,INPUT);
  pinMode(ch3pin,INPUT);
  pinMode(ch4pin,INPUT);
  pinMode(ch5pin,INPUT);
  pinMode(ch6pin,INPUT);

  servo1.attach(servo2pin);
  servo1.write(servo1Home);

  servo2.attach(servo2pin);
  servo2.write(servo2Home);
 
  pinMode(lightsPin,OUTPUT);
  digitalWrite(lightsPin,LOW);
 
}

int ch1;
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

int way = RELEASE;
int noway = RELEASE;
boolean doDelay = false;
boolean lightsOn = false;
boolean fullStop = true;


// the loop function runs over and over again forever
void loop() {
  doDelay = false;

  ch1 = pulseIn(ch1pin,HIGH,25000);
  ch2 = pulseIn(ch2pin,HIGH,25000);
  ch3 = pulseIn(ch3pin,HIGH,25000);
  ch4 = pulseIn(ch4pin,HIGH,25000);
  ch5 = pulseIn(ch5pin,HIGH,25000);
  ch6 = pulseIn(ch6pin,HIGH,25000);

  ch1val = map(ch1,1100,1920,-10,10); // camera left/rigt
  ch2val = map(ch2,1100,1920,-10,10); // camera up/down

  ch3val = map(ch3,1100,1920,0,maxSpeed); 
  if (ch3val < 0) ch3val = 0;
  if (ch3val > 255) ch3val = 255;

  ch4val = map(ch4,1100,1920,-50,50); // turn
  if (ch4val < -90) ch4val = -50;
  if (ch4val >  90) ch4val =  50;

  ch5val = map(ch5,900,2100,BACKWARD,FORWARD);  // forward, backward
 
  //ch6val = map(ch6,1100,1920,-1,1); // mode select
  ch6val = 1;

  // debug start //
  Serial.print(ch1);
  Serial.print(" ");
  Serial.print(ch2);
  Serial.print(" ");
  Serial.print(ch3);
  Serial.print(" ");
  Serial.print(ch4);
  Serial.print(" ");
  Serial.print(ch5);
  Serial.print(" ");
  Serial.println(ch6);
  // debug end //
  
  // robot forward, backward + gear
  if (ch3val > 0) {
    motor1->run(way);
    motor2->run(way);
    motor3->run(way);
    motor4->run(way);
    motor1->setSpeed(ch3val);
    motor2->setSpeed(ch3val);
    motor3->setSpeed(ch3val);
    motor4->setSpeed(ch3val);
    fullStop = false;
  } else {
    motor1->run(RELEASE);
    motor2->run(RELEASE);
    motor3->run(RELEASE);
    motor4->run(RELEASE);
    fullStop = true;
    if (ch5val == FORWARD) {
      way = FORWARD;
      noway = BACKWARD;
    } else {
      way = FORWARD;
      noway = BACKWARD;    
    }
  }
  
  // robot turning
 turn(ch4val);

  if (ch6val > 0) {  // camera mode on
  
    // camera left/right
    if (ch1val > 0) {
      int servo1val = servo1.read();
      if (servo1val+ch1val > maxServo1) {
        servo1.write(maxServo1);
      } else {
        servo1.write(servo1val+ch1val);
      }
      doDelay = true;
    } 
    if (ch1val < 0) {
      int servo1val = servo1.read();
      if (servo1val-ch1val < minServo1) {
        servo1.write(minServo1);
      } else {
        servo1.write(servo1val-ch1val);
      }  
      doDelay = true;
    } 
  
    // camera up/down
    if (ch2val > 0) {
      int servo2val = servo2.read();
      if (servo2val+ch2val > maxServo2) {
        servo2.write(maxServo2);
      } else {
        servo2.write(servo2val+ch2val);
      }
      doDelay = true;
    } 
    if (ch2val < 0) {
      int servo2val = servo2.read();
      if (servo2val-ch1val < minServo2) {
        servo2.write(minServo2);
      } else {
        servo2.write(servo2val-ch2val);
      }  
      doDelay = true;
    } 
  
  } else {  // setupmode on

    if (ch1val == -10) {  // camera to home
       servo1.write(servo1Home);
       servo2.write(servo2Home);
       doDelay = true;
     } else if (ch1val == 10) {   // lights on/off
       if (lightsOn) {
         lightsOn = false;
         digitalWrite(lightsPin,LOW);
       } else {
         lightsOn = true;
         digitalWrite(lightsPin,HIGH);
       }
     } else if (ch2val == -10) {
     } else if (ch2val == 10) {
     }
   }
  
  
  // delay for servos
  if (doDelay) {
    delay(15);  
  }
 
 }

void turn (int val) 
{
  if (val > 0) { //right
    if (fullStop) {
      motor1->run(way);
      motor2->run(way);
      motor3->run(noway);
      motor4->run(noway);
      motor1->setSpeed(val);
      motor2->setSpeed(val);
      motor3->setSpeed(val);
      motor4->setSpeed(val);
    } else {
      int turnVal = ch3val-val;
      if (turnVal < 0) {
        motor3->run(noway);
        motor4->run(noway);
      } else {
        motor3->run(way);
        motor4->run(way);
      }
      motor3->setSpeed(turnVal);
      motor4->setSpeed(turnVal);
    }
  } else {  // left
    if (fullStop) {
      motor1->run(noway);
      motor2->run(noway);
      motor3->run(way);
      motor4->run(way);
      motor1->setSpeed(val);
      motor2->setSpeed(val);
      motor3->setSpeed(val);
      motor4->setSpeed(val);
    } else {
      int turnVal = ch3val-val;
      if (turnVal < 0) {
        motor1->run(noway);
        motor2->run(noway);
      } else {
        motor1->run(way);
        motor2->run(way);
      }
      motor1->setSpeed(turnVal);
      motor2->setSpeed(turnVal);
    }  }
}



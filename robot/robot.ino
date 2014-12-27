/*

 */
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

#define maxSpeed 200

#define minServo1 20
#define maxServo1 159
#define minServo2 72
#define maxServo2 169
#define servo1Home 100
#define servo2Home 100

#define ch1pin 2 
#define ch1maxval 5
#define ch2pin 3
#define ch2maxval 5
#define ch3pin 4
#define ch4pin 5
#define ch4maxval 10
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

  servo1.attach(servo1pin);
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
boolean lightsCanToggle = true;
boolean fullStop = true;


// the loop function runs over and over again forever
void loop() {

  delay(100);

  ch1 = pulseIn(ch1pin,HIGH,25000);
  ch2 = pulseIn(ch2pin,HIGH,25000);
  ch3 = pulseIn(ch3pin,HIGH,25000);
  ch4 = pulseIn(ch4pin,HIGH,25000);
  ch5 = pulseIn(ch5pin,HIGH,25000);
  ch6 = pulseIn(ch6pin,HIGH,25000);

  ch1val = map(ch1,1100,1920,-ch1maxval,ch1maxval+1); // camera left/rigt
  ch2val = map(ch2,1100,1920,-ch2maxval,ch2maxval+1); // camera up/down

  ch3val = map(ch3,1300,1850,-10,maxSpeed); 
  if (ch3val < 0) ch3val = 0;
  if (ch3val > maxSpeed) ch3val = maxSpeed;
  ch3val = maxSpeed - ch3val;
  
  ch4val = map(ch4,1000,2000,-ch4maxval,ch4maxval+1); // turn
  if (ch4val < -10) ch4val = -10;
  if (ch4val >  10) ch4val =  10;
  
  if(ch5 > 1500) { // forward, backward
    ch5val = BACKWARD;
  } else {
    ch5val = FORWARD;
  }
  
  if(ch6 > 1500) { // forward, backward
    ch6val = -1;
  } else {
    ch6val = 1;
  }
  
  //ch5val = map(ch5,1000,1900,FORWARD,BACKWARD);  
 
  //ch6val = map(ch6,1100,1920,-1,1); // mode select

  // debug start //
//  Serial.print(ch1val);
//  Serial.print(" ");
//  Serial.print(ch2val);
//  Serial.print("     ");
//  Serial.print(ch3val);
//  Serial.print("     ");
//  Serial.print(ch4val);
//  Serial.print("     ");
//  Serial.print(ch5val);
//  Serial.print(" ");
//  Serial.print(ch6val);
//  Serial.print(" ");
//  Serial.print(servo1.read());
//  Serial.print(" ");
//  Serial.println(servo2.read());
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
      way = BACKWARD;
      noway = FORWARD;    
    }
  }
  
  // robot turning
  turn(ch4val);

  if (ch6val > 0) {  // camera mode on
  
    // camera left/right
    if (ch1val < 0) {
      int servo1val = servo1.read();
      if (servo1val+ch1val > maxServo1) {
        servo1.write(servo1val);
      } else {
        servo1.write(servo1val+abs(ch1val));
      }
    } 
    if (ch1val > 0) {
      int servo1val = servo1.read();
      if (servo1val-ch1val < minServo1) {
        servo1.write(servo1val);
      } else {
        servo1.write(servo1val-abs(ch1val));
      }  
    } 
  
    // camera up/down
    if (ch2val < 0) {
      int servo2val = servo2.read();
      if (servo2val+ch2val > maxServo2) {
        servo2.write(servo2val);
      } else {
        servo2.write(servo2val+abs(ch2val));
      }
    } 
    if (ch2val > 0) {
      int servo2val = servo2.read();
      if (servo2val-ch1val < minServo2) {
        servo2.write(servo2val);
      } else {
        servo2.write(servo2val-abs(ch2val));
      }  
    } 
  
  } else {  // setupmode on

    if (ch2val == -ch2maxval) {  // camera to home
       servo1.write(servo1Home);
       servo2.write(servo2Home);
    } else if (ch2val == ch2maxval) {   // lights on/off
      if (lightsCanToggle) {
        if (lightsOn) {
          lightsCanToggle = false;
          lightsOn = false;
          digitalWrite(lightsPin,LOW);
        } else {
          lightsCanToggle = false;
          lightsOn = true;
          digitalWrite(lightsPin,HIGH);
        }
      }
    } else if (ch1val == -ch1maxval) {
      //tba 
    } else if (ch1val == ch1maxval) {
      //tba 
    } else {
      lightsCanToggle = true;
    }
  }
}

void turn (int val) 
{
  if (val > 0) { //right
    val = abs(val)*10;
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
      motor3->setSpeed(abs(turnVal));
      motor4->setSpeed(abs(turnVal));
    }
  } else {  // left
    val = abs(val)*10;
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
      motor1->setSpeed(abs(turnVal));
      motor2->setSpeed(abs(turnVal));
    }  }
}



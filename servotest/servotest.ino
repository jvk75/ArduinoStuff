/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

#include "Servo.h"

#define sA 2
#define sB 3
#define sC 4
#define sD 5
#define sE 6
#define sF 7
#define sG 8
#define sH 9
//#define BOMB 10
#define sMotor 10
#define sRX 11

Servo myservo;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600); 
  // initialize digital pin 13 as an output.
  pinMode(sA, OUTPUT);
  pinMode(sB, OUTPUT);
  pinMode(sC, OUTPUT);
  pinMode(sD, OUTPUT);
  pinMode(sE, OUTPUT);
  pinMode(sF, OUTPUT);
  pinMode(sG, OUTPUT);
  pinMode(sH, OUTPUT);
  //pinMode(BOMB, OUTPUT);
  
  myservo.attach(sMotor);
  pinMode(sRX,INPUT);
}

int val;
int newval;
int ch3;

// the loop function runs over and over again forever
void loop() {
  ch3 = pulseIn(sRX,HIGH,25000);
  
  //Serial.println(ch1);
  //Serial.println(myservo.attached());
  //digitalWrite(BOMB, LOW);
  //val = myservo.read();
  //myservo.write(120);
  val = map(ch3,1110,1915,55,175);
  newval = map(ch3,1110,1900,0,9);
  Serial.println(val);
  myservo.write(val);
  runNumber(newval);

  delay(10);
}


void clearAll() {
  digitalWrite(sA, LOW);
  digitalWrite(sB, LOW);
  digitalWrite(sC, LOW);
  digitalWrite(sD, LOW);
  digitalWrite(sE, LOW);
  digitalWrite(sF, LOW);  
  digitalWrite(sG, LOW);
  digitalWrite(sH, LOW);
}

void runNumber(int num) {
  clearAll();
  switch (num) {
    case 0:
      digitalWrite(sA,HIGH);
      digitalWrite(sB,HIGH);
      digitalWrite(sC,HIGH);
      digitalWrite(sD,HIGH);
      digitalWrite(sE,HIGH);
      digitalWrite(sF,HIGH);
      break;
    case 1:
      digitalWrite(sB,HIGH);
      digitalWrite(sC,HIGH);
      break;
    case 2:
      digitalWrite(sA,HIGH);
      digitalWrite(sB,HIGH);
      digitalWrite(sG,HIGH);
      digitalWrite(sE,HIGH);
      digitalWrite(sD,HIGH);
      break;
    case 3:
      digitalWrite(sA,HIGH);
      digitalWrite(sB,HIGH);
      digitalWrite(sG,HIGH);
      digitalWrite(sC,HIGH);
      digitalWrite(sD,HIGH);
      break;
    case 4:
      digitalWrite(sF,HIGH);
      digitalWrite(sG,HIGH);
      digitalWrite(sB,HIGH);
      digitalWrite(sC,HIGH);
      break;
    case 5:
      digitalWrite(sA,HIGH);
      digitalWrite(sF,HIGH);
      digitalWrite(sG,HIGH);
      digitalWrite(sC,HIGH);
      digitalWrite(sD,HIGH);
      break;
    case 6:
      digitalWrite(sA,HIGH);
      digitalWrite(sF,HIGH);
      digitalWrite(sG,HIGH);
      digitalWrite(sC,HIGH);
      digitalWrite(sD,HIGH);
      digitalWrite(sE,HIGH);
      break;
    case 7:
      digitalWrite(sA,HIGH);
      digitalWrite(sB,HIGH);
      digitalWrite(sC,HIGH);
      break;
    case 8:
      digitalWrite(sA,HIGH);
      digitalWrite(sB,HIGH);
      digitalWrite(sC,HIGH);
      digitalWrite(sD,HIGH);
      digitalWrite(sE,HIGH);
      digitalWrite(sF,HIGH);
      digitalWrite(sG,HIGH);
      break;
    default: //case 9:
      digitalWrite(sA,HIGH);
      digitalWrite(sB,HIGH);
      digitalWrite(sC,HIGH);
      digitalWrite(sD,HIGH);
      digitalWrite(sF,HIGH);
      digitalWrite(sG,HIGH);
      break;
  }
}



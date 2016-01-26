/*
* temp relay logger by JVK75
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <dht.h>

//uncomment if wifi in use
//#define WIFI 

// pins
#define waterTempPin 12
#define airTempHumidPin 10
#define relayPin 4
#define wifiReadyLed 13
#define wifiReadyPin A1
#define wifiTxPin 8
#define wifiRxPin 9
#define potPin A2

// calibration
#define waterTempCalibration 1.0
#define airTempCalibration 1.0
#define airHumidCalibration 0.0

//limits
#define waterHighLimit 0
#define waterLowLimit 0

#define timeLowLimit 1
#define timeMaxLimit 45

#define aDay 86400

// definitions
OneWire waterTempOneWire(waterTempPin);
DallasTemperature waterTemp(&waterTempOneWire);
dht airTempHumid;
SoftwareSerial wifiSerial(wifiRxPin, wifiTxPin); // RX, TX

// setup
void setup(void)
{
  // start serial ports
  Serial.begin(115200);
  wifiSerial.begin(115200);
  
  pinMode(relayPin,OUTPUT);
  pinMode(wifiReadyLed,OUTPUT);

  digitalWrite(relayPin,LOW);
  digitalWrite(wifiReadyLed,LOW);
  
  // Start up the dallaslibrary
  waterTemp.begin();
  
  Serial.println("start");
  
  delay(100);
}

bool isConnect = false;
unsigned long previousMillis = 0;        
int relayState = LOW;
long relayDelay = 1;
int potDelay = 0;

void loop(void)
{ 
  unsigned long currentMillis = millis();
   
  if(currentMillis - previousMillis >= relayDelay) {
    potDelay = map(analogRead(potPin),0,1023,timeLowLimit,timeMaxLimit);
    Serial.print("pd: ");
    Serial.println(potDelay);
  
 
    previousMillis = currentMillis;   
    if (relayState == LOW) {
      relayState = HIGH;
      relayDelay = potDelay*1000;
    } else {
      relayState = LOW;
      relayDelay = aDay*1000;
    }
    digitalWrite(relayPin, relayState);
    Serial.println(relayDelay);
  }

 
#ifndef WIFI
    delay(100);
#else
  if (!isConnect) {
    waitReadyPin(wifiReadyPin);
    //sendToWifi("connect Arduino:teeseite");
    sendToWifi("connect Arduino:teeseite");
    waitReadyPin(wifiReadyPin);

    sendToWifi("privatekey Ww5KbXEVPVf80xZpYWA5");
    waitReadyPin(wifiReadyPin);

    sendToWifi("publickey 6Jqr16OdNdHb0JGZV2Xx");
    waitReadyPin(wifiReadyPin);
    
    isConnect = true;
    digitalWrite(wifiReadyLed, HIGH);
  } else {
    waitReadyPin(wifiReadyPin);
    
    waterTemp.requestTemperatures();
    float wt = waterTemp.getTempCByIndex(0) - waterTempCalibration;
    float at;
    float ah;
    int chk = airTempHumid.read22(airTempHumidPin);
    if (chk == DHTLIB_OK) {
      at = airTempHumid.temperature - airTempCalibration;
      ah = airTempHumid.humidity - airHumidCalibration;
    } else {
      at = 0;
      ah = 0;
    }
       
    String data = "data ";
    data += "\"airtemp\":\"";
    data += at;
    data += "\",\"airhumid\":\"";
    data += ah;
    data += "\",\"watertemp\":\"";
    data += wt;
    data += "\",\"relay\":\"";
    data += digitalRead(relayPin) ? 10 : 0;
    data += "\",\"relay_high\":\"";
    data += potDelay;
    data += "\",\"relay_low\":\"";
    data += waterLowLimit;
    data += "\"";
    
    sendToWifi(data);

    delay(5000);
  }
#endif
}

void checkRelay(float wt)
{
  /*if (wt > waterHighLimit && digitalRead(relayPin) == HIGH) {
    digitalWrite(relayPin,LOW);
  } else if (wt < waterLowLimit && digitalRead(relayPin) == LOW) {
    digitalWrite(relayPin,HIGH);
  }*/
}

void sendToWifi(String str)
{
  Serial.println(str);
  wifiSerial.println(str);
  delay(1000);
}

void waitWifiSerial() 
{
  if (wifiSerial.available()) {
    while(wifiSerial.available()){
      char c = wifiSerial.read();
      Serial.print(c);
    }
  }
}

void waitReadyPin(int pin)
{
  while(analogRead(pin) < 900) {
    delay(500);  
    Serial.println("Waiting readyPin");
  }
}


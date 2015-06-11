/*
* temp relay logger by JVK75
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <dht.h>

// pins
#define waterTempPin 12
#define airTempHumidPin 10
#define relayPin 4
#define wifiReadyPin A1
#define wifiTxPin 8
#define wifiRxPin 9

// calibration
#define waterTempCalibration 1.0
#define airTempCalibration 1.0
#define airHumidCalibration 0.0

//limits
#define waterHighLimit 30
#define waterLowLimit 24


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

  digitalWrite(relayPin,LOW);
  
  // Start up the dallaslibrary
  waterTemp.begin();
  
  Serial.println("start");
  
  delay(100);
}

bool isConnect = false;

void loop(void)
{ 
  waitReadyPin(wifiReadyPin);
  
  if (!isConnect) {
    sendToWifi("connect Arduino:teeseite");
    waitReadyPin(wifiReadyPin);

    sendToWifi("privatekey rzemWDaYXeFyBm1l04Gz");
    waitReadyPin(wifiReadyPin);

    sendToWifi("publickey AJrago6ADrf6AZRr0Wa1");
    waitReadyPin(wifiReadyPin);
    
    isConnect = true;
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
    
    checkRelay(wt);
    
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
    data += waterHighLimit;
    data += "\",\"relay_low\":\"";
    data += waterLowLimit;
    data += "\"";
    
    sendToWifi(data);

    delay(15000);
  }
}

void checkRelay(float wt)
{
  if (wt > waterHighLimit && digitalRead(relayPin) == HIGH) {
    digitalWrite(relayPin,LOW);
  } else if (wt < waterLowLimit && digitalRead(relayPin) == LOW) {
    digitalWrite(relayPin,HIGH);
  }
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
  while(analogRead(pin) < 900) {delay(100);  /*Serial.println("Waiting readyPin");*/}
}


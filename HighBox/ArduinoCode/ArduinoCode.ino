#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
#include <dht.h>


// pins
#define waterTempPin 12
#define airTempHumidPin 10
#define relayPin 4

// calibration
#define waterTempCalibration 1.0

//
OneWire waterTempOneWire(waterTempPin);
DallasTemperature waterTemp(&waterTempOneWire);

dht airTempHumid;


SoftwareSerial wifiSerial(9, 8); // RX, TX


void setup(void)
{
  // start serial port
  Serial.begin(115200);
  pinMode(relayPin,OUTPUT);
  digitalWrite(relayPin,LOW);
  // Start up the library
  waterTemp.begin();
}

void loop(void)
{ 
  delay(2000);
  // call waterTemp.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  waterTemp.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  Serial.print("Temperature for the device 1 (index 0) is: ");
  float wt = waterTemp.getTempCByIndex(0) - waterTempCalibration;
  Serial.println(wt);  
  
//  int chk = airTempHumid.read22(waterTempPin);
//  if (chk == DHTLIB_OK) {
//    Serial.print("Air temp: ");
//    Serial.print(airTempHumid.temperature, 1);
//    Serial.print(" - Air humid: ");
//    Serial.println(airTempHumid.humidity, 1);
//  } else {
//    Serial.println(chk);
//  }

digitalWrite(relayPin,HIGH);
delay(2000);
digitalWrite(relayPin,LOW);

}

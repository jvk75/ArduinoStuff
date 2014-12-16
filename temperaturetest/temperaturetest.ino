// This Arduino sketch reads DS18B20 "1-Wire" digital
// temperature sensors.
// Tutorial:
// http://www.hacktronics.com/Tutorials/arduino-1-wire-tutorial.html

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 3 on the Arduino
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer = { 0x28, 0x01, 0x50, 0xDC, 0x04, 0x00, 0x00, 0x31 };

#include <dht.h>

dht DHT;
int chk;

#define lightPin A0
#define moistPin A1
#define ledPin 7
int lightVal;
int moistVal;

#define DHT21_PIN 5

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  // Start up the library
  sensors.begin();
  // set the resolution to 10 bit (good enough?)
  sensors.setResolution(insideThermometer, 10);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  pinMode(ledPin,OUTPUT);
  }

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC == -127.00) {
    Serial.print("Error getting temperature");
  } else {
    Serial.print("C: ");
    Serial.print(tempC);
  }
}

void loop(void)
{ 
  delay(500);
  digitalWrite(ledPin,LOW);
  Serial.print("Getting temperatures...\n\r");
  sensors.requestTemperatures();
  
  Serial.print("Inside temperature is: ");
  printTemperature(insideThermometer);
  Serial.print("\n\r");
  
  // READ DATA
  Serial.print("DHT21, \t");
  chk = DHT.read21(DHT21_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
		Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.print("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.print("Time out error,\t"); 
		break;
    default: 
		Serial.print("Unknown error,\t"); 
		break;
  }
 // DISPLAY DATA
  Serial.print(DHT.humidity,1);
  Serial.print(",\t");
  Serial.println(DHT.temperature,1);
  
  lightVal = analogRead(lightPin);
  moistVal = analogRead(moistPin);
  Serial.print("Light: ");
  Serial.println(lightVal);
  if (lightVal > moistVal) {
    digitalWrite(ledPin,HIGH);
  }
  Serial.print("Pot: ");
  Serial.println(moistVal);
  
  Serial.println();
 }


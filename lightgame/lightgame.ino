
#define lightPin A0
#define potPin A1
#define ledPin 7
int lightVal;
int potVal;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
  }


void loop(void)
{ 
  //delay(10);
  lightVal = analogRead(lightPin);
  potVal = analogRead(potPin);
  //Serial.print("Light: ");
  //Serial.println(lightVal);
  if (lightVal > potVal) {
    digitalWrite(ledPin,HIGH);
  } else {
    digitalWrite(ledPin,LOW);
  }
  //Serial.print("Pot: ");
  //Serial.println(potVal);
  
  //Serial.println();
 }


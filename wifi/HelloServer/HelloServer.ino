#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
const char* ssid = "xxx";
const char* password = "xxx";


ESP8266WebServer server(80);
 
const int led = 2;
 
void handle_root() {
  //digitalWrite(led, 1);
  server.send(200, "text/html", "<!DOCTYPE html><html><head></head><body>Hello from esp8266!<br>Light: <a href=\"on\">on</a>, <a href=\"off\">off</a></body></html>");
  //delay(100);
  //digitalWrite(led, 0);
}
 
void setup(void)
{
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led, 1);
    delay(200);
    digitalWrite(led, 0);
    delay(200);
    digitalWrite(led, 1);
    delay(200);
    digitalWrite(led, 0);
    Serial.print(".");
  }
  digitalWrite(led, 1);

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   
  server.on("/", handle_root);
  
  server.on("/on", [](){
    server.send(200, "text/html", "<!DOCTYPE html><html><head><meta http-equiv=\"refresh\" content=\"0; url=.\"></head><body></body></html>");
    digitalWrite(led, 1);
  });
  server.on("/off", [](){
    server.send(200, "text/html", "<!DOCTYPE html><html><head><meta http-equiv=\"refresh\" content=\"0; url=.\"></head><body></body></html>");
    digitalWrite(led, 0);
  });
   
  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void)
{
  server.handleClient();
} 




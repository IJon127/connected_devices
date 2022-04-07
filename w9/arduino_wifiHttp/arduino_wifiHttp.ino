/**************************************************************************
  Wifi http client
  Uses Tom Igoe's TestHttpClient: https://github.com/tigoe/MakingThingsTalk2/blob/main/3rd_edition/chapter4/TestHttpClient/TestHttpClient.ino
  created 05 Apr 2022
  modified 07 Apr 2022
  by I-Jon Hsieh
 **************************************************************************/
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

#include "secret.h"


WiFiClient netSocket;
const char server[] = "ijon.xyz";
String route = "/";
int portNumber = 80;



void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  //waiting for connection
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("I'm trying to connect to ");
    Serial.println(SECRET_SSID);
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
  }

  //connected! print out the device's network status
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

}


void loop() {
  Serial.println("making request");
  HttpClient http(netSocket, server, portNumber);
  http.get(route);

  while(http.connected()){
    Serial.println("connected");
    Serial.println(http.available());
    if(http.available()){
      String result = http.readString();
      Serial.print(result);
    }
  }

  http.stop();
  delay(5000);
}

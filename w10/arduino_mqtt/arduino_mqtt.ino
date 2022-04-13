/**************************************************************************
  Mqtt client button led
  Uses Tom Igoe's MqttClientButtonLed: https://github.com/tigoe/mqtt-examples/blob/main/MqttClientButtonLed/MqttClientButtonLed.ino
  
  This sketch uses https://shiftr.io/try as the MQTT broker.
  
  the circuit:
  - pushbutton attached to pin 2, connected to ground
  - LED's anode connected to pin 3, cathode connected to ground.
  
  created 12 Apr 2022
  modified 12 Apr 2022
  by I-Jon Hsieh
 **************************************************************************/
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include "secret.h"

//initialize wifi connection:
WiFiClient wifi;
MqttClient mqttClient(wifi);

//detials for MQTT client:
char broker[] = "public.cloud.shiftr.io";
int port = 1883;
char topic[] = "lights";
char clientID[] = "buttonClient";

//initialized LED state:
int intensity = 0;

//details for pushbutton and LED:
const int buttonPin = 2;
const int ledPin = 3;
const int debounceDelay = 5;
int lastButtonState = 0;


void setup() {
  Serial.begin(9600);
  while(!Serial);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  while (WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connecting to ");
    Serial.println(SECRET_SSID);
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    delay(2000);
  }

  Serial.print("Connected. My IP address: ");
  Serial.println(WiFi.localIP());

  mqttClient.setId(clientID);
  mqttClient.setUsernamePassword(SECRET_MQTT_USER, SECRET_MQTT_PASS);

  while (!connectToBroker()){
    Serial.println("Attempting to connect to broker");
    delay(1000);
  }

  Serial.println("Connected to broker");
}



void loop() {
  //if not connectd to the broker, try to connect:
  if (!mqttClient.connected()){
    Serial.println("reconnecting");
    connectToBroker();
  }

  if (mqttClient.parseMessage() > 0){
    Serial.print("Got a message on topic: ");
    Serial.println(mqttClient.messageTopic());

    while (mqttClient.available()){
      int message = mqttClient.parseInt();
      Serial.println(message);

      if (message > 0){
        intensity = 255;
      }
    }
  }

  //if LED is on
  if (intensity > 0){
    analogWrite(ledPin, intensity);
    intensity--;
  }

  //read pushbutton
  int buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState){
    delay(debounceDelay);

    if(buttonState == LOW){
      mqttClient.beginMessage(topic);
      mqttClient.print(random(16));
      mqttClient.endMessage();
    }
    
    lastButtonState = buttonState;
  }
  
}


boolean connectToBroker(){
  //if MQtt client is not connected:
  if (!mqttClient.connect(broker, port)){
    Serial.print("MQTT connection failed. Error no: ");
    Serial.println(mqttClient.connectError());

    return false;
  }

  //once you're connected, process...
  mqttClient.subscribe(topic);

  return true;
}

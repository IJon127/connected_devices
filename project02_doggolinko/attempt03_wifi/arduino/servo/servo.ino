/**************************************************************************
  Pet picture frames
  Uses Tom Igoe's MqttClientButtonLed: https://github.com/tigoe/mqtt-examples/blob/main/MqttClientButtonLed/MqttClientButtonLed.ino
  DIY Electronics Projects'Arduino with PIR Motion Sensor: https://create.arduino.cc/projecthub/biharilifehacker/arduino-with-pir-motion-sensor-fd540a
  Arduino Servo example Knob: http://www.arduino.cc/en/Tutorial/Knob
  Arduino Debounce example: https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
  created 19 April 2022
  modified 5 May 2022
  by I-Jon Hsieh
 **************************************************************************/
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include "secrets.h"

#include <Servo.h>

//initialize wifi connection:
WiFiClient wifi;
MqttClient mqttClient(wifi);

//detials for MQTT client:
char broker[] = "public.cloud.shiftr.io";
int port = 1883;
char topic[] = "doggolinko";
char clientID[] = "servoClient";


const int servoPin = 2;

const int wifiLedPin = 7;           // wifi connected led indicator
const int brokerLedPin = 4;           // mqtt broker connected led indicator
const int errorLedPin = 5;
const int dataInLedPin = 6;

Servo myservo;
float servoAngle = 43;
const int closeAngle = 43;
const int openAngle = 147;      //should be 90 degree. However,each servo might be different
float closingSpeed = 0.001;      //the decreasing angle of each loop
                  

unsigned long lastTimer;    //the previous time sumState was HIGH
unsigned long waitingTime = 5000;  //stay open for certain milliseconds

int sumState;

int randomNum = 10;
unsigned long lastMillis = 0;


void setup() {
  pinMode(wifiLedPin, OUTPUT); 
  pinMode(brokerLedPin, OUTPUT);      
  pinMode(errorLedPin, OUTPUT); 
  pinMode(dataInLedPin, OUTPUT);
  myservo.attach(servoPin);

  digitalWrite(wifiLedPin, LOW);
  digitalWrite(brokerLedPin, LOW);
  digitalWrite(errorLedPin, LOW);
  digitalWrite(dataInLedPin, LOW);

  Serial.begin(9600);                 // initialize serial
 


  mqttClient.setId(clientID);
  mqttClient.setUsernamePassword(SECRET_MQTT_USER, SECRET_MQTT_PASS);


  connectToBroker();

  while (!connectToBroker()){
    Serial.println("Attempting to connect to broker");
    delay(1000);
  }

  Serial.println("Connected to broker");
  digitalWrite(brokerLedPin, HIGH);
}



void loop() {
  //MQTT connection:
    
  if (!mqttClient.connected()){       //if not connectd to the broker, try to connect
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
        lastMillis = millis();
        if(message > randomNum){
          sumState = HIGH;
        }
      }
    }
  }

  if (millis() - lastMillis > 10000) { //Didn't receive any message within 10 sec
    digitalWrite(dataInLedPin, LOW);
  } else {
    digitalWrite(dataInLedPin, HIGH);
  }

  

  if (sumState == HIGH) {
    Serial.println("OPEN!!!");
    lastTimer = millis();
    servoAngle = openAngle;
  }

  if ((millis() - lastTimer) > waitingTime) {
    if (servoAngle > closeAngle) {
      servoAngle -= closingSpeed;
    } else {
      servoAngle = closeAngle;
    }
  }

  myservo.write(servoAngle);
  Serial.println(servoAngle);

  sumState = LOW;
}




//my functions---------------------------------------

boolean connectToBroker(){
  checkWifiConnection();
  
  //if MQtt client is not connected:
  if (!mqttClient.connect(broker, port)){
    Serial.print("MQTT connection failed. Error no: ");
    Serial.println(mqttClient.connectError());
    digitalWrite(brokerLedPin, LOW);
    
    return false;
  }

  //once you're connected, process...
  mqttClient.subscribe(topic);
  digitalWrite(brokerLedPin, HIGH);


  return true;
}

void checkWifiConnection(){
    while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connecting to ");
    Serial.println(SECRET_SSID);
    WiFi.begin(SECRET_SSID, SECRET_PASS);
    digitalWrite(wifiLedPin, HIGH);
    delay(500);
    digitalWrite(wifiLedPin, LOW);
    delay(500);
    digitalWrite(wifiLedPin, HIGH);
    delay(500);
    digitalWrite(wifiLedPin, LOW);
    delay(500);
  }

  Serial.print("Connected. My IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(wifiLedPin, HIGH);
}

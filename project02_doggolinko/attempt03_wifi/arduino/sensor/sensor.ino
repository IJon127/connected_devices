/**************************************************************************
  Pet detecting sensors
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


//initialize wifi connection:
WiFiClient wifi;
MqttClient mqttClient(wifi);

//detials for MQTT client:
char broker[] = "public.cloud.shiftr.io";
int port = 1883;
char topic[] = "doggolinko";
char clientID[] = "sensorClient";

const int lowerSensorPin = 4;           // lower sensor
const int higherSensorPin = 2;          // higher sensor
const int lowerLedPin = 5;           // lower sensor led indicator
const int higherLedPin = 3;           // higher sensor led indicator
const int wifiLedPin = 6;           // wifi connected led indicator
const int brokerLedPin = 7;           // mqtt broker connected led indicator
const int errorLedPin = 8;          // mqtt error led indicator


int state1 = LOW;               // by default, no motion detected
int state2 = LOW;               // by default, no human detected
int lowerVal = LOW;                 // variable to store the red button status (value)
int higherVal = LOW;                 // variable to store the yellow button status (value)



int sumState;                  // the current data from the summary of sensor 1 and 2
int lastSumState = LOW;        // the previous data from the summary of sensor 1 and 2
unsigned long lastDebounceTime = 0;  // the last time the servo was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

unsigned long lastMillis = 0;
int randomNum = 10;




void setup() {
  pinMode(lowerSensorPin, INPUT);         
  pinMode(higherSensorPin, INPUT);         
  pinMode(lowerLedPin, OUTPUT);         
  pinMode(higherLedPin, OUTPUT);   
  pinMode(wifiLedPin, OUTPUT); 
  pinMode(brokerLedPin, OUTPUT);      
  pinMode(errorLedPin, OUTPUT);       

  
  digitalWrite(lowerLedPin, LOW);
  digitalWrite(higherLedPin, LOW);
  digitalWrite(wifiLedPin, LOW);
  digitalWrite(brokerLedPin, LOW);
  digitalWrite(errorLedPin, LOW);

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

  //Sensor Data:
  lowerVal = digitalRead(lowerSensorPin);   // read lower sensor value
  higherVal = digitalRead(higherSensorPin);   // read highersensor value
  Serial.print("lower: ");
  Serial.print(lowerVal);
  Serial.print("  higher: ");
  Serial.println(higherVal);
  
  if (lowerVal == HIGH){
    digitalWrite(lowerLedPin, HIGH);
  } else {
    digitalWrite(lowerLedPin, LOW);
  }
  if (higherVal == HIGH){
    digitalWrite(higherLedPin, HIGH);
  } else{
    digitalWrite(higherLedPin, LOW);
  }

  state1 = checkSensorState(lowerVal, state1);
  state2 = checkSensorState(higherVal, state2);
  sumStateDebounce();
  
  //MQTT connection:
  
  if (!mqttClient.connected()){          //if not connectd to the broker, try to connect:
    Serial.println("reconnecting");
    connectToBroker();
  }
  
  //Sent timer to MQTT broker:
   if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    mqttClient.beginMessage(topic);
    mqttClient.print(random(randomNum));
    mqttClient.endMessage();
  }
  
  //Sent sumState to MQTT broker:
  
  if (sumState == HIGH) {
      Serial.println("open!");
      mqttClient.beginMessage(topic);
      mqttClient.print(random(randomNum+10, randomNum+20));
      mqttClient.endMessage();
  } 
}




//my functions---------------------------------------

int checkSensorState(int sensorVal, int state) {
  if (sensorVal == HIGH) {           // check if the sensor is HIGH
    delay(200);                        // delay 200 milliseconds
    if (state == LOW) {
      state = HIGH;                    // update variable state to HIGH
    }
  }
  else {
    delay(200);                        // delay 200 milliseconds
    if (state == HIGH) {
      state = LOW;                     // update variable state to LOW
    }
  }
  return state;
}


void sumStateDebounce() {
  int tempSumState;

  if (state1 == HIGH && state2 == LOW) {
    tempSumState = HIGH;
  } else {
    tempSumState = LOW;
  }

  if (tempSumState != lastSumState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (tempSumState != sumState) {
      sumState = tempSumState;
    }
  }

  lastSumState = tempSumState;
}



boolean connectToBroker(){
  checkWifiConnection();
  
  //if MQtt client is not connected:
  if (!mqttClient.connect(broker, port)){
    Serial.print("MQTT connection failed. Error no: ");
    Serial.println(mqttClient.connectError());
    digitalWrite(brokerLedPin, LOW);
    digitalWrite(errorLedPin, HIGH);
    
    return false;
  }

  //once you're connected, process...
  mqttClient.subscribe(topic);
  digitalWrite(brokerLedPin, HIGH);
  digitalWrite(errorLedPin, LOW);


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

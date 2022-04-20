/**************************************************************************
  Pet detecting sensors
  Uses Tom Igoe's MqttClientButtonLed: https://github.com/tigoe/mqtt-examples/blob/main/MqttClientButtonLed/MqttClientButtonLed.ino
  DIY Electronics Projects'Arduino with PIR Motion Sensor: https://create.arduino.cc/projecthub/biharilifehacker/arduino-with-pir-motion-sensor-fd540a
  Arduino Servo example Knob: http://www.arduino.cc/en/Tutorial/Knob
  Arduino Debounce example: https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
  created 19 April 2022
  modified 19 April 2022
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
char topic[] = "linkingDoggo";
char clientID[] = "sensorClient";

const int sensor1Pin = 4;           // the pin that the red button is atteched to
const int sensor2Pin = 5;           // the pin that the yellow button is atteched to


int state1 = LOW;               // by default, no motion detected
int state2 = LOW;               // by default, no human detected
int val1 = LOW;                 // variable to store the red button status (value)
int val2 = LOW;                 // variable to store the yellow button status (value)



int sumState;                  // the current data from the summary of sensor 1 and 2
int lastSumState = LOW;        // the previous data from the summary of sensor 1 and 2
unsigned long lastDebounceTime = 0;  // the last time the servo was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers





void setup() {
  pinMode(sensor1Pin, INPUT);         // initialize red button as an input
  pinMode(sensor2Pin, INPUT);         // initialize yellow button as an input

  Serial.begin(9600);                 // initialize serial

//  while(!Serial);

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
  //MQTT connection:
  
  if (!mqttClient.connected()){          //if not connectd to the broker, try to connect:
    Serial.println("reconnecting");
    connectToBroker();
  }


  //Sensor Data:
  val1 = digitalRead(sensor1Pin);   // read red button value
  val2 = digitalRead(sensor2Pin);   // read yellow button value

  state1 = checkSensorState(val1, state1);
  state2 = checkSensorState(val2, state2);
  sumStateDebounce();

  
  //Sent sumState to MQTT broker:
  
  if (sumState == HIGH) {
      Serial.println("open!");
      mqttClient.beginMessage(topic);
      mqttClient.print(1);
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

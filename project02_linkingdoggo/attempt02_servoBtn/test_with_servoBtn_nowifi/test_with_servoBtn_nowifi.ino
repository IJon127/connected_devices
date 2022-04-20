/**************************************************************************
  Uses DIY Electronics Projects'Arduino with PIR Motion Sensor:https://create.arduino.cc/projecthub/biharilifehacker/arduino-with-pir-motion-sensor-fd540a
  Arduino Servo example Knob: http://www.arduino.cc/en/Tutorial/Knob
  Arduino Debounce example: https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
  created 19 April 2022
  modified 19 April 2022
  by I-Jon Hsieh
 **************************************************************************/
#include <Servo.h>

const int sensor1Pin = 4;           // the pin that the red button is atteched to
const int sensor2Pin = 5;           // the pin that the yellow button is atteched to
const int servoPin = 2;

int state1 = LOW;               // by default, no motion detected
int state2 = LOW;               // by default, no human detected
int val1 = LOW;                 // variable to store the red button status (value)
int val2 = LOW;                 // variable to store the yellow button status (value)

Servo myservo;
float servoAngle = 0;
const int openAngle = 97;      //should be 90 degree. However,each servo might be different
float closingSpeed = 0.5;      //the decreasing angle of each loop

int sumState;                  // the current data from the summary of sensor 1 and 2
int lastSumState = LOW;        // the previous data from the summary of sensor 1 and 2
unsigned long lastDebounceTime = 0;  // the last time the servo was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

unsigned long lastTimer;    //the previous time sumState was HIGH
unsigned long waitingTime = 5000;  //stay open for certain milliseconds




void setup() {
  pinMode(sensor1Pin, INPUT);         // initialize red button as an input
  pinMode(sensor2Pin, INPUT);         // initialize yellow button as an input

  myservo.attach(servoPin);

  Serial.begin(9600);                 // initialize serial
}

void loop() {
  val1 = digitalRead(sensor1Pin);   // read red button value
  val2 = digitalRead(sensor2Pin);   // read yellow button value

  state1 = checkSensorState(val1, state1);
  state2 = checkSensorState(val2, state2);



  sumStateDebounce();

  if (sumState == HIGH) {
    Serial.println("OPEN!!!");
    lastTimer = millis();
    servoAngle = 97;
    myservo.write(servoAngle);
  }

  if ((millis() - lastTimer) > waitingTime) {
    if (servoAngle > 0) {
      servoAngle -= closingSpeed;
    } else {
      servoAngle = 0;
    }
  }

  myservo.write(servoAngle);
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

/**************************************************************************
  Uses DIY Electronics Projects'Arduino with PIR Motion Sensor:https://create.arduino.cc/projecthub/biharilifehacker/arduino-with-pir-motion-sensor-fd540a
  and Arduino Servo example Knob: http://www.arduino.cc/en/Tutorial/Knob
  created 19 April 2022
  modified 19 April 2022
  by I-Jon Hsieh
 **************************************************************************/
#include <Servo.h>

const int sensor1Pin = 4;              // the pin that the red button is atteched to
const int sensor2Pin = 5;           // the pin that the yellow button is atteched to
const int servoPin = 2;

int state1 = LOW;               // by default, no motion detected
int state2 = LOW;               // by default, no human detected
int val1 = LOW;                 // variable to store the red button status (value)
int val2 = LOW;                 // variable to store the yellow button status (value)

float servoAngle = 0;

Servo myservo;



void setup() {
  pinMode(sensor1Pin, INPUT);         // initialize red button as an input
  pinMode(sensor2Pin, INPUT);         // initialize yellow button as an input

  myservo.attach(servoPin);

  Serial.begin(9600);                 // initialize serial
}

void loop() {
  val1 = digitalRead(sensor1Pin);   // read red button value
  val2 = digitalRead(sensor2Pin);   // read yellow button value

  state1 = checkSensorState(val1, state1, "motion");
  state2 = checkSensorState(val2, state2, "human");


  if (state1 == HIGH && state2 == LOW) {
    Serial.println("OPEN!!!");
    servoAngle = 90;
  }
  if (servoAngle > 0) {
    servoAngle-=0.5;
  } else {
    servoAngle = 0;
  }
  myservo.write(servoAngle);
}



int checkSensorState(int sensorVal, int state, String subject) {
  if (sensorVal == HIGH) {           // check if the sensor is HIGH
    
    delay(100);                        // delay 100 milliseconds
    if (state == LOW) {
      Serial.println(subject);
      state = HIGH;                    // update variable state to HIGH
    }
  }
  else {
    
    delay(200);                        // delay 200 milliseconds
    if (state == HIGH) {
      Serial.print("no ");
      Serial.print(subject);
      Serial.println(" is detected!");
      state = LOW;                     // update variable state to LOW
    }
  }
  return state;
}

/**************************************************************************
  Uses DIY Electronics Projects'Arduino with PIR Motion Sensor:https://create.arduino.cc/projecthub/biharilifehacker/arduino-with-pir-motion-sensor-fd540a
  created 19 April 2022
  modified 19 April 2022
  by I-Jon Hsieh
 **************************************************************************/
const int greenLedPin = 2;         // the pin that the green LED is atteched to
const int blueLedPin = 3;          // the pin that the blue LED is atteched to

const int sensor1Pin = 4;              // the pin that the red button is atteched to
const int sensor2Pin = 5;           // the pin that the yellow button is atteched to

int state1 = LOW;               // by default, no motion detected
int state2 = LOW;               // by default, no human detected
int val1 = LOW;                 // variable to store the red button status (value)
int val2 = LOW;                 // variable to store the yellow button status (value)





void setup() {
  pinMode(greenLedPin, OUTPUT);       // initalize green LED as a digital output
  pinMode(sensor1Pin, INPUT);         // initialize red button as an input
  pinMode(sensor2Pin, INPUT);         // initialize yellow button as an input
  Serial.begin(9600);                 // initialize serial
}

void loop() {
  val1 = digitalRead(sensor1Pin);   // read red button value
  val2 = digitalRead(sensor2Pin);   // read yellow button value

  state1 = checkSensorState(val1, greenLedPin, state1, "motion");
  state2 = checkSensorState(val2, blueLedPin, state2, "human");


  if(state1 == HIGH && state2 == LOW){
    Serial.println("OPEN!!!");
  }
}



int checkSensorState(int sensorVal, int ledPin, int state, String subject){
    if (sensorVal == HIGH) {           // check if the sensor is HIGH
    digitalWrite(ledPin, HIGH);        // turn LED ON
    delay(100);                        // delay 100 milliseconds
    if (state == LOW) {
      Serial.println(subject);
      state = HIGH;                    // update variable state to HIGH
    }
  }
  else {
    digitalWrite(ledPin, LOW);         // turn LED OFF
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

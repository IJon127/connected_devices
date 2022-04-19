/**************************************************************************
  Uses DIY Electronics Projects'Arduino with PIR Motion Sensor:https://create.arduino.cc/projecthub/biharilifehacker/arduino-with-pir-motion-sensor-fd540a
  created 19 April 2022
  modified 19 April 2022
  by I-Jon Hsieh
 **************************************************************************/
const int greenLedPin = 2;         // the pin that the green LED is atteched to
const int blueLedPin = 3;          // the pin that the blue LED is atteched to

const int redBtnPin = 4;              // the pin that the red button is atteched to
const int yellowBtnPin = 5;           // the pin that the yellow button is atteched to

int state = LOW;             // by default, no motion detected
int redBtnVal = 0;                 // variable to store the red button status (value)
int yellowBtnVal = 0;                 // variable to store the yellow button status (value)

int dogIsMoving = 0;        // by default, no dog detected

String subject;    // variable to store the subject being detected (human/dog)




void setup() {
  pinMode(greenLedPin, OUTPUT);      // initalize green LED as a digital output
  pinMode(redBtnPin, INPUT);         // initialize red button as an input
  pinMode(yellowBtnPin, INPUT);      // initialize yellow button as an input
  Serial.begin(9600);                // initialize serial
}

void loop() {
  redBtnVal = digitalRead(redBtnPin);   // read red button value
  yellowBtnVal = digitalRead(yellowBtnPin);   // read yellow button value

  if (redBtnVal == HIGH) {
    if (yellowBtnVal == LOW) {
      subject = "Dog";

      if (dogIsMoving == LOW) {
        dogIsMoving = HIGH;
        Serial.println("Moving Started! ");
      }
    } else {
      subject = "Human";
    }
        Serial.print(subject);
        Serial.println(" detected!");
  } else {
    dogIsMoving = LOW;
  }





  /*
    if (val == HIGH) {           // check if the sensor is HIGH
    digitalWrite(greenLedPin, HIGH);   // turn LED ON
    Serial.println("H!");
    delay(100);                // delay 100 milliseconds


    if (state == LOW) {
      Serial.println("Motion detected!");
      state = HIGH;       // update variable state to HIGH
    }
    }
    else {
      digitalWrite(greenLedPin, LOW); // turn LED OFF
      Serial.println("L!");
      delay(500);             // delay 200 milliseconds

      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
    }
    }
  */
}

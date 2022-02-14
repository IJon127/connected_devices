/**************************************************************************
  Game Controller (4 push btn)
  Uses Arduino example: https://www.arduino.cc/en/Tutorial/BuiltInExamples/KeyboardReprogram
  created 10 Feb 2022
  modified 14 Feb 2022
  by I-Jon Hsieh
 **************************************************************************/
#include "Keyboard.h"

//use this for OSX.
char ctrlKey = KEY_LEFT_GUI;

int const upBtnPin = 5; //yellow
int const downBtnPin = 4; //brown
int const leftBtnPin = 3; //blue
int const rightBtnPin = 2; //red

int upBtn = 1;
int downBtn = 1;
int leftBtn = 1;
int rightBtn = 1;
int lastUpBtnState = 1;
int lastDownBtnState = 1;
int lastLeftBtnState = 1;
int lastRightBtnState = 1;

void setup() {
  Serial.begin(9600);
  //  Serial.println("hello");

  pinMode(upBtnPin, INPUT_PULLUP);
  pinMode(downBtnPin, INPUT_PULLUP);
  pinMode(leftBtnPin, INPUT_PULLUP);
  pinMode(rightBtnPin, INPUT_PULLUP);


  Keyboard.begin();
}

void loop() {
  upBtn = digitalRead(upBtnPin);
  downBtn = digitalRead(downBtnPin);
  leftBtn = digitalRead(leftBtnPin);
  rightBtn = digitalRead(rightBtnPin);

  matchingBtn(upBtn, &lastUpBtnState, KEY_UP_ARROW);
  matchingBtn(downBtn, &lastDownBtnState, KEY_DOWN_ARROW);
  matchingBtn(leftBtn, &lastLeftBtnState, KEY_LEFT_ARROW);
  matchingBtn(rightBtn, &lastRightBtnState, KEY_RIGHT_ARROW);

}


void matchingBtn(int btnState, int *lastBtnState, int keyName) {

  if (btnState == 0) {
    if (btnState != *lastBtnState) {
      Keyboard.press(keyName);
      Keyboard.releaseAll();
//      Serial.println(keyName);
    }
  }
  *lastBtnState = btnState;
}

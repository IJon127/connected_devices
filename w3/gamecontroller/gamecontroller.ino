/**************************************************************************
  Game Controller (4 push btn)
  Uses Arduino example: https://www.arduino.cc/en/Tutorial/BuiltInExamples/KeyboardReprogram
  created 10 Feb 2022
  modified 10 Feb 2022
  by I-Jon Hsieh
 **************************************************************************/
#include "Keyboard.h"

//use this for OSX.
char ctrlKey = KEY_LEFT_GUI;

int const upBtnPin = 5;
int const downBtnPin = 4;
int const leftBtnPin = 3;
int const rightBtnPin = 2;

int upBtn = 0;
int downBtn = 0;
int leftBtn = 0;
int rightBtn = 0;
int LastUpBtnState = 0;
int LastDownBtnState = 0;
int LastLeftBtnState = 0;
int LastRightBtnState = 0;

void setup() {
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

  matchingBtn(upBtn, &LastUpBtnState, KEY_UP_ARROW);
  matchingBtn(downBtn, &LastDownBtnState, KEY_DOWN_ARROW);
  matchingBtn(leftBtn, &LastLeftBtnState, KEY_LEFT_ARROW);
  matchingBtn(rightBtn, &LastRightBtnState, KEY_RIGHT_ARROW);

  //  if (upBtn == LOW) {
  //    Keyboard.press(KEY_UP_ARROW);
  //  }
  //
  //  if (downBtn == LOW) {
  //    Keyboard.press(KEY_DOWN_ARROW);
  //  }
  //
  //  if (leftBtn == LOW) {
  //    Keyboard.press(KEY_LEFT_ARROW);
  //  }
  //
  //  if (rightBtn == LOW) {
  //    Keyboard.press(KEY_RIGHT_ARROW);
  //  }

}


void matchingBtn(int btnState, int *lastBtnState, int keyName) {

  if (btnState == LOW) {
    if (btnState != *lastBtnState) {
//      Keyboard.press(keyName);
       *lastBtnState = btnState;
    }
  }

}

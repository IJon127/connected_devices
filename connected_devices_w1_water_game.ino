#include <Arduino_JSON.h> //!!!!

const int fsrPin = A0;  
const int potPin = A1;

int lastFsrValue = 0;        
int lastPotValue = 0; 
int fsrThreshold = 20;
int potThreshold = 25;  

JSONVar device; //!!!!

void setup() {
  Serial.begin(9600);
  device["fsr"] = 0;
  device["pot"] = 0;
}

void loop() {
  int fsrValue = analogRead(fsrPin);
  int potValue = analogRead(potPin);

//fsr has changed ------------
  if(abs(fsrValue - lastFsrValue) > fsrThreshold){
    device["fsr"] = fsrValue;
    lastFsrValue = fsrValue;
  }

// pot has changed ----------
  if(abs(potValue - lastPotValue) > potThreshold){
    device["pot"] = potValue;
    lastPotValue = potValue;
  }

  Serial.println(device);
}

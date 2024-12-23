#include <Arduino.h>
#include "input.h"

volatile int enterPresses = 0;
volatile int upPresses = 0;
volatile int downPresses = 0;

volatile int lastEnterInterrupt = 0;
volatile int lastUpInterrupt = 0;
volatile int lastDownInterrupt = 0;

void enterButtonISR();
void upButtonISR();
void downButtonISR();

const int enterPin = 18;
const int upPin = 2;
const int downPin = 3;

void setupTest(){
    pinMode(enterPin, INPUT_PULLUP);
    pinMode(downPin, INPUT_PULLUP);
    pinMode(upPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(enterPin), enterButtonISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(downPin), downButtonISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(upPin), upButtonISR, FALLING);
}

bool buttonHasBeenPressed(Button b) {
  if (b == ENTER & enterPresses > 0) {
    enterPresses -= 1;
    return true;
  } else if (b == UP & upPresses > 0) {
    upPresses -= 1;
    return true;
  } else if (b == DOWN & downPresses >0) {
    downPresses -= 1;
    return true;
  } else {
    return false;
  }
}

void enterButtonISR(){
  int currentTime = millis();

  if((currentTime - lastEnterInterrupt) < 200){
    return;
  }
  
  lastEnterInterrupt = currentTime;
  enterPresses += 1;
}

void upButtonISR(){
  int currentTime = millis();

  if(currentTime - lastUpInterrupt < 200){
    return;
  }

  lastUpInterrupt = currentTime;
  upPresses += 1;
}

void downButtonISR(){
  int currentTime = millis();

  if(currentTime - lastDownInterrupt < 200){
    return;
  }

  lastDownInterrupt = currentTime;
  downPresses += 1;
}
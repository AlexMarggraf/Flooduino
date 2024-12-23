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

void setupTest(){
    pinMode(3, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(3), upButtonISR, FALLING);
}

bool buttonHasBeenPressed(Button b){
  if(b == ENTER & enterPresses > 0){
    enterPresses -= 1;
    return true;
  }else if(b == UP & upPresses > 0){
    upPresses -= 1;
    return true;
  }else if(b == DOWN & downPresses >0){
    downPresses -= 1;
    return true;
  }else{
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

  if(currentTime - lastUpInterrupt < 200){
    return;
  }

  lastDownInterrupt = currentTime;
  downPresses += 1;
}
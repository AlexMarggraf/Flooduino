#include <Arduino.h>
#include "input.h"

volatile static int enterPresses = 0;
volatile static int upPresses = 0;
volatile static int downPresses = 0;

volatile static long lastEnterInterrupt = 0;
volatile static long lastUpInterrupt = 0;
volatile static long lastDownInterrupt = 0;

void enterButtonISR();
void upButtonISR();
void downButtonISR();

const int enterPin = 20;
const int upPin = 2;
const int downPin = 3;

void setupInput() {
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
  } else if (b == DOWN & downPresses > 0) {
    downPresses -= 1;
    return true;
  } else {
    return false;
  }
}

void enterButtonISR() {
  long currentTime = millis();

  if ((currentTime - lastEnterInterrupt) < 250) {
    return;
  }

  lastEnterInterrupt = currentTime;
  enterPresses += 1;
}

void upButtonISR() {
  long currentTime = millis();

  if (currentTime - lastUpInterrupt < 250) {
    return;
  }

  lastUpInterrupt = currentTime;
  upPresses += 1;
}

void downButtonISR() {
  long currentTime = millis();

  if (currentTime - lastDownInterrupt < 250) {
    return;
  }

  lastDownInterrupt = currentTime;
  downPresses += 1;
}
#include "input.h"
#include "output.h"

void setup() {
  setupTest();
  Serial.begin(9600);
}

void loop() {
  if (buttonHasBeenPressed(ENTER)) {
    Serial.print("Enter has been pressed!");
    Serial.println(millis());
  } else if (buttonHasBeenPressed(UP)) {
    Serial.print("Up has been pressed!");
    Serial.println(millis());
  } else if (buttonHasBeenPressed(DOWN)) {
    Serial.print("Down has been pressed!");
    Serial.println(millis());
  }
}
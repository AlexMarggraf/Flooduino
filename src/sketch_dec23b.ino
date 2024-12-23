#include "input.h"

void setup() {
  setupTest();
  Serial.begin(9600);
}

void loop() {
    if(buttonHasBeenPressed(ENTER)){
      Serial.print("Enter has been pressed! ");
      Serial.print(millis());
      Serial.println("");
    }else if(buttonHasBeenPressed(UP)){
      Serial.print("Up has been pressed! ");
      Serial.print(millis());
      Serial.println("");
    }
}
#include <stdbool.h>

// this header contains declarations of an interface for using buttons as input

#ifndef INPUT_H
#define INPUT_H

typedef enum Button {
    ENTER,
    UP,
    DOWN
} Button;

void setupTest();

/**
 * Checks, whether the specified button has been pressed since last checked. 
 */
bool buttonHasBeenPressed(Button);

#endif // INPUT_H
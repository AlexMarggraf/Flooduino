#include <stdbool.h> 

// this header contains declarations of an interface for using buttons as input

#ifndef INPUT_H
#define INPUT_H

typedef enum Button {
    ENTER,
    UP,
    DOWN
} Button;

bool ButtonHasBeenPressed(Button);

#endif // INPUT_H

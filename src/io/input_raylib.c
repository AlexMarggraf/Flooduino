// this file implements the functions declared in output.h using raylib

#include "raylib.h"
#include "../common.h"
#include "input.h"


bool ButtonHasBeenPressed(Button b) {
    if(b == ENTER){
        return IsKeyPressed(257);
    } else if (b == DOWN){
        return IsKeyPressed(264);
    } else{
        return IsKeyPressed(265);
    }
}
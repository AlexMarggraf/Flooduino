/*******************************************************************************************
*
*   raylib game template
*
*   <Game title>
*   <Game description>
*
*   This game has been created using raylib (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "common.h"    // NOTE: Declares global (extern) variable and types
#include "io/input.h"
#include "io/output.h"

//----------------------------------------------------------------------------------
// Shared Variable Definition (global)
// NOTE: This variable is shared between modules through common.h
//----------------------------------------------------------------------------------
GameState gameState = { 0 };


void SetTestState(void);

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    InitScreen("Prototype Flooduino");
    SetTestState();
    UpdateLayout();

    // Main game loop
    while (!ShouldEndGame())    // Detect window close button or ESC key
    {
        // TODO implement logic here (this code was only for testing the visuals)
        if (ButtonHasBeenPressed(UP)) {
            gameState.fieldSize++;
            UpdateLayout();
        }
        if (ButtonHasBeenPressed(DOWN)) {
            gameState.fieldSize--;
            UpdateLayout();
        }
        if (ButtonHasBeenPressed(ENTER)) {
            gameState.numberOfMoves++;
            UpdateLayout();
        }
        DrawScreen();
    }

    TerminateScreen();

    return 0;
}

/**
 * This function is only for testing purposes. 
 * It sets the global variable to a 
 */
void SetTestState() {
    gameState = (GameState){ 
        .screen = GAME, 
        .field = {
            5, 1, 0, 5, 2, 6, 0, 3, 3, 2, 7, 6, 4, 0, 2, 1, 5, 3, 5, 3, 3, 0, 2, 1, 4, 6, 0, 5, 2, 2, 4, 0, 1, 7, 7, 0, 6, 7, 7, 3, 0, 3, 2, 5, 1, 1, 0, 7, 1, 7, 4, 7, 7, 3, 5, 4, 7, 6, 7, 1, 2, 7, 0, 0, 2, 2, 5, 5, 3, 0, 3, 6, 0, 2, 0, 3, 7, 5, 4, 3, 7, 6, 1, 1, 7, 1, 3, 7, 4, 2, 3, 7, 2, 7, 3, 4, 6, 1, 2, 2, 
            7, 6, 5, 5, 1, 3, 6, 7, 6, 5, 0, 5, 5, 3, 6, 5, 6, 4, 3, 2, 3, 4, 6, 7, 3, 6, 7, 5, 0, 7, 1, 0, 2, 2, 5, 2, 5, 2, 7, 4, 2, 0, 7, 5, 3, 1, 0, 3, 0, 5, 0, 1, 3, 7, 0, 5, 6, 4, 5, 4, 0, 2, 1, 1, 1, 1, 2, 2, 5, 1, 7, 1, 6, 6, 3, 4, 0, 6, 7, 2, 1, 1, 5, 2, 0, 4, 7, 5, 0, 2, 3, 1, 3, 0, 6, 2, 0, 3, 7, 4, 
            0, 0, 7, 3, 4, 2, 3, 5, 7, 1, 1, 7, 2, 7, 3, 3, 4, 5, 4, 7, 3, 1, 3, 2, 2, 5, 6, 6, 5, 5, 2, 6, 1, 0, 5, 6, 0, 1, 0, 2, 4, 6, 5, 0, 7, 3, 3, 4, 1, 7, 4, 5, 7, 1, 5, 4, 4, 7, 4, 4, 3, 5, 7, 3, 4, 1, 4, 0, 7, 7, 0, 4, 0, 6, 7, 3, 6, 7, 2, 1, 1, 5, 7, 7, 1, 1, 4, 7, 4, 2, 2, 1, 2, 0, 3, 5, 6, 1, 1, 4, 
            6, 5, 2, 1, 3, 5, 7, 2, 2, 4, 4, 7, 2, 2, 3, 2, 4, 2, 6, 6, 7, 5, 6, 3, 0, 2, 1, 7, 4, 1, 1, 1, 4, 5, 4, 4, 0, 2, 1, 0, 0, 2, 6, 6, 7, 1, 4, 2, 5, 7, 3, 6, 7, 5, 5, 6, 1, 4, 1, 7, 3, 1, 6, 6, 6, 2, 7, 5, 6, 4, 4, 3, 7, 2, 5, 5, 7, 2, 6, 7, 0, 6, 1, 2, 2, 6, 2, 1, 7, 2, 7, 5, 6, 1, 7, 0, 7, 0, 3, 1, 
            6, 3, 5, 0, 5, 1, 3, 1, 4, 6, 7, 5, 7, 2, 7, 2, 0, 6, 3, 3, 6, 6, 3, 1, 0, 4, 4, 2, 3, 1, 4, 5, 7, 7, 4, 0, 7, 0, 7, 3, 6, 1, 3, 0, 5, 7, 1, 0, 0, 2, 4, 1, 0, 6, 2, 2, 6, 3, 5, 0, 1, 1, 7, 7, 0, 1, 6, 1, 4, 0, 2, 5, 2, 1, 6, 2, 6, 1, 1, 4, 4, 4, 2, 6, 6, 1, 6, 4, 3, 7, 3, 3, 6, 6, 3, 0, 7, 0, 1, 0, 
            0, 3, 1, 7, 2, 4, 0, 0, 3, 0, 5, 7, 5, 2, 2, 1, 2, 1, 3, 5, 4, 0, 7, 2, 1, 2, 3, 3, 4, 2, 1, 4, 0, 0, 5, 4, 3, 2, 1, 4, 0, 1, 2, 3, 4, 5, 4, 1, 5, 7, 1, 3, 2, 0, 4, 6, 0, 2, 4, 3, 4, 7, 2, 1, 1, 6, 2, 5, 5, 6, 6, 5, 3, 6, 4, 5, 2, 4, 3, 2, 7, 7, 7, 0, 4, 7, 0, 4, 5, 0, 0, 3, 5, 7, 6, 6, 2, 1, 3, 2, 
            0, 0, 4, 6, 5, 2, 2, 6, 3, 6, 5, 4, 1, 2, 2, 2, 4, 7, 0, 2, 7, 2, 5, 6, 2, 7, 2, 2, 1, 0, 4, 0, 7, 2, 7, 7, 4, 2, 2, 1, 6, 3, 5, 5, 0, 2, 7, 0, 2, 7, 7, 3, 6, 1, 7, 7, 0, 3, 3, 6, 4, 0, 2, 4, 1, 0, 1, 5, 3, 3, 3, 5, 2, 0, 5, 7},
        .fieldSize = 10,
        .numberOfColors = 8,
        .numberOfMoves = 0,
        .currentColor = 1
    };
}

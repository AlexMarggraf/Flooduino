
// this header contains declarations of an interface for using a screen (either implemented for Raylib, or the LED-Screen)

#include <stdbool.h>
#ifndef OUTPUT_H
#define OUTPUT_H

/** 
 * This function has to be called once at the start of the application to initialize the screen.
 * @param[in] title this is the title of the window (only relevant for Raylib)
 */
void InitScreen(const char* title);

/** 
 * Has to be called after initializing gameState and when gameState.fieldSize changed.
 */
void UpdateLayout(void); 

/** 
 * Draws the screen according to the current gameState.
 */
void DrawScreen(void);

/**
 * Has to be called when the application ends. 
 */
void TerminateScreen(void);

/**
 * Returns whether the user wants to close the window.
 * (only relevant for implementation with raylib)
 * @returns true, iff the close button on the window or ESC has been pressed.
 */
bool ShouldEndGame(void);

#endif // OUTPUT_H

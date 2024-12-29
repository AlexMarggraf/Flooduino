
// this header contains declarations of an interface for using a screen (either implemented for Raylib, or the LED-Screen)

#include <stdbool.h>
#ifndef OUTPUT_H
#define OUTPUT_H

/** 
 * This function has to be called once at the start of the application to initialize the screen.
 * @param[in] title this is the title of the window (only relevant for Raylib)
 */
void InitScreen(void);  // TODO remove this parameter

/** 
 * Has to be called after initializing gameState and when gameState.fieldSize changed.
 */
void UpdateLayout(void);

/** 
 * Draws the screen according to the current gameState.
 */
void DrawScreen(void);

#endif  // OUTPUT_H

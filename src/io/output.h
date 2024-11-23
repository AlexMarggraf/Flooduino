
// this header contains declarations of an interface for using a screen

#include <stdbool.h>
#ifndef OUTPUT_H
#define OUTPUT_H

void InitScreen(const char*);
void UpdateLayout(void); // has to be called when gameState.fieldSize changed and after initializing gameState
void DrawScreen(void);
void TerminateScreen(void);
bool ShouldEndGame(void);

#endif // OUTPUT_H

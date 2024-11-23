
#ifndef COMMON_H
#define COMMON_H

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef enum GameScreen {
    TITLE,
    COLOR_SELECTION,
    SIZE_SELECTION,
    GAME,
    WON,
    LOST,
} GameScreen;
typedef struct GameState { 
    GameScreen screen;
    char field[26][26];
    int fieldSize;
    int numberOfColors;
    int numberOfMoves;
    int currentColor;
} GameState;

//----------------------------------------------------------------------------------
// Global Variables Declaration (shared by several modules)
//----------------------------------------------------------------------------------
extern GameState gameState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Various Function Declarations
//----------------------------------------------------------------------------------
void DrawScreen(void);
void InitGameState(void);
void UpdateGameState(void);
// TODO

#ifdef __cplusplus
}
#endif

#endif // SCREENS_H
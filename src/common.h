
#ifndef COMMON_H
#define COMMON_H

// This header file declares global things (Types and a global variable)

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

// Global Variable Declaration (shared by all modules)
extern GameState gameState;

#endif // COMMON_H
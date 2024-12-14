
#ifndef COMMON_H
#define COMMON_H

/** 
 * This header file contains all declarations relevant for the whole game. 
 * At the moment it is just the game state that is declared here. 
 */ 


/**
 * This is the datatype describing which screen the game is on at the moment. 
 */
typedef enum GameScreen {
    TITLE,
    COLOR_SELECTION,
    SIZE_SELECTION,
    GAME,
    WON,
    LOST,
} GameScreen;


/**
 * This datatype represents the game state. 
 * it contains the following members:
 * - screen: which screen the game is on at the moment, 
 * - field: an array containing the current colors, 
 * - fieldSize: the size of the field, 
 * - numberOfColors: the number of colors the player can choose from,
 * - numberOfMoves: the number of moves the player has done,
 * - currentColor: the color that the player has selected at the moment, 
 * - maxNumberOfMoves: the maximal number of moves a player can make to still win.
 */
typedef struct GameState { 
    GameScreen screen;
    char field[26][26];
    int fieldSize;
    int numberOfColors;
    int numberOfMoves;
    int currentColor;
    int maxNumberOfMoves;
} GameState;

/**
 * Struct representing a point in a two-dimensional array
 */
typedef struct {
    int x;
    int y;
} Point;

// This is the global variable representing the current state of the game. 
extern GameState gameState;

#endif // COMMON_H
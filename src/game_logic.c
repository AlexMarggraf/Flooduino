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
#include <stdlib.h>
// TODO: Projekt

//----------------------------------------------------------------------------------
// Shared Variable Definition (global)
// NOTE: This variable is shared between modules through common.h
//----------------------------------------------------------------------------------
GameState gameState = { 0 };


void SetTestState(void);
void flood_fill(int x, int y, int original_color, int new_color);
int** gameBoardFixed(void);
void setNumberOfMoves(void); //TODO: implement this function
bool inField(int x, int y);
bool isWithinRange(int x);
void increaseFieldSize(void);
void decreaseFieldSize(void);
bool is_won(int board[26][26]);
int** gameBoardFixed(void);
void freeGameboard(void);
void selectionModeSize(void);
void selectionModeColors(void);
void increaseColors(void);
void decreaseColors(void);


int numberOfMoves;
int fieldSizeCounter;
int** gameBoard;

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    InitScreen("Prototype Flooduino");
    SetTestState();
    UpdateLayout();

    // Main game loops
    selectionModeSize();
    selectionModeColors();

    TerminateScreen();
    return 0;
}

/**
 * This function is only for testing purposes. 
 * It sets the global gameState variable to some state. 
 * Feel free to modify this function for testing different states of the game. 
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


 void flood_fill(int x, int y, int original_color, int new_color){
    // Falsche Farbe oder bereits geflutet
    freeGameboard();
    gameBoard = gameBoardFixed();
    if(!inField(x,y)){
        return;
    }
    if (gameBoard[x][y] != original_color || gameBoard[x][y] == new_color) {
        return;
    }

    // Farbe ändern
    gameBoard[x][y] = new_color;

    // Nachbarn prüfen
    flood_fill(x + 1, y, original_color, new_color);
    flood_fill(x - 1, y, original_color, new_color);
    flood_fill(x, y + 1, original_color, new_color);
    flood_fill(x, y - 1, original_color, new_color);
}
void setNumberOfMoves(){
    gameState.numberOfMoves = gameState.fieldSize;
}

bool inField(int x, int y){
    // Grenzen prüfen
    if (x < 0 || x >= 26 || y < 0 || y >= 26) {
        return false;
    }
    else{
        return true;
    }
}

bool isWithinRange(int x){
    if (x < 0 || x >= 26){
        return false;
    }
    else{
        return true;
    }
}

void increaseFieldSize(){
     if(gameState.fieldSize == 26){ //TODO: Was ist die Minimale Grösse?
            gameState.fieldSize = 8;
            }
            else{
                gameState.fieldSize++;
            }
}

void decreaseFieldSize(){
    if(gameState.fieldSize == 8){ //TODO: Was ist die Minimale Grösse?
    gameState.fieldSize = 26;
    }
    else{
        gameState.fieldSize--;
    }
}

bool is_won(int board[26][26]) { // TODO: Array dynamisch machen.
    int color = board[0][0];
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            if (board[i][j] != color) {
                return 0; // Noch nicht gewonnen
            }
        }
    }
    return 1; // Gewonnen
}

int** gameBoardFixed(){
    fieldSizeCounter = gameState.fieldSize;

    // Dynamisches 2D-Array erstellen
    int** gameBoard = malloc(fieldSizeCounter * sizeof(int*)); // Zeilenzeiger allokieren
    if (gameBoard == NULL) {
        printf("Fehler: Speicher konnte nicht allokiert werden.\n");
        return NULL;
    }

    for (int i = 0; i < fieldSizeCounter; i++) {
        gameBoard[i] = malloc(fieldSizeCounter * sizeof(int)); // Spalten für jede Zeile allokieren
        if (gameBoard[i] == NULL) {
            printf("Fehler: Speicher konnte nicht allokiert werden.\n");
            // Bereits allokierten Speicher freigeben
            for (int j = 0; j < i; j++) {
                free(gameBoard[j]);
            }
            free(gameBoard);
            return NULL;
        }
    }
    return gameBoard;
}

void freeGameboard(){
    if(gameBoard != NULL){
        for (int i = 0; i < fieldSizeCounter; i++) {
        free(gameBoard[i]); // Spalten freigeben
    }
    free(gameBoard);
    }
    
}

void selectionModeSize(){
        while (!ShouldEndGame()) {
        // TODO implement logic here (this code was only for testing the visuals)
        if (ButtonHasBeenPressed(UP)) {
            increaseFieldSize();
           UpdateLayout();
        }
        if (ButtonHasBeenPressed(DOWN)) {
            decreaseFieldSize();
            UpdateLayout();
        }
        if (ButtonHasBeenPressed(ENTER)) {
            setNumberOfMoves();
            UpdateLayout();
            DrawScreen();
            return;
        }
        DrawScreen();

    }
}

void selectionModeColors(){
    while (!ShouldEndGame()) {
        // TODO implement logic here (this code was only for testing the visuals)
        if (ButtonHasBeenPressed(UP)) {
            increaseFieldSize();
           UpdateLayout();
        }
        if (ButtonHasBeenPressed(DOWN)) {
            decreaseFieldSize();
            UpdateLayout();
        }
        if (ButtonHasBeenPressed(ENTER)) {
            setNumberOfMoves();
            UpdateLayout();
            gameMode();
            DrawScreen();
            return;
        }
        DrawScreen();

    }

}

void increaseColors(){
     if(gameState.fieldSize == 26){ //TODO: Was ist die Minimale Grösse?
            gameState.fieldSize = 8;
            }
            else{
                gameState.fieldSize++;
            }
}

void decreaseColors(){
    if(gameState.numberOfColors == 3){ //TODO: Was ist die Minimale Grösse?
    gameState.numberOfColors = 7;
    }
    else{
        gameState.numberOfColors--;
    }
}




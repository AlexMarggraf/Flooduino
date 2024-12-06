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
#include <stdbool.h>

//----------------------------------------------------------------------------------
// Shared Variable Definition (global)
// NOTE: This variable is shared between modules through common.h
//----------------------------------------------------------------------------------
GameState gameState = { 0 };


void SetGamefield(void);
void floodFillIterative();
void setNumberOfMoves(void); //TODO: implement this function
bool inField(int x, int y);
bool isWithinRange(int x);
void increaseFieldSize(void);
void decreaseFieldSize(void);
bool is_won();
void selectionModeSize(void);
void selectionModeColors(void);
void titleScreen(void);
void gameMode(void);
void increaseNumberOfColors(void);
void decreaseNumberOfColors(void);
void selectColorInGameUP(void);
void selectColorInGameDOWN(void);
void wonScreen(void);
void lostScreen(void);
void SetInitialState(void);


int numberOfMoves;
int fieldSizeCounter;
int** gameBoard;
int chosenNumberOfColor;
int maxSize;

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // main game loop
    SetInitialState();
    InitScreen("Prototype Flooduino");
    UpdateLayout();
    while (!ShouldEndGame()) {
        titleScreen();
        gameState.numberOfColors = 3;
        selectionModeSize();
        gameState.currentColor = 2;
        selectionModeColors();
        chosenNumberOfColor = gameState.numberOfColors;
        SetGamefield();
        gameMode();
    }

    TerminateScreen();
    return 0;
}

/**
 * This function is only for testing purposes. 
 * It sets the global gameState variable to some state. 
 * Feel free to modify this function for testing different states of the game. 
 */
void SetGamefield() {
    srand(time(NULL));
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            gameState.field[i][j] = rand() % gameState.numberOfColors;
        }
    }
}

void SetInitialState() {
    gameState = (GameState) { 
        .screen = GAME, 
        .fieldSize = 10,
        .numberOfColors = 8,
        .numberOfMoves = 0,
        .currentColor = 1
    };
    srand(time(NULL));
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            gameState.field[i][j] = rand() % 8;
        }
    }
}

// Struktur für Koordinaten
typedef struct {
    int x;
    int y;
} Point;

// Funktion, um den Flood-It-Algorithmus auszuführen
void floodFillIterative() {
    int originalColor = gameState.field[0][0]; // Die ursprüngliche Farbe oben links

    // Warteschlange für Punkte (Koordinaten), die noch verarbeitet werden müssen
    Point queue[26 * 26];
    int front = 0, back = 0;

    // Füge den Startpunkt (oben links) zur Warteschlange hinzu
    queue[back++] = (Point) {0, 0};

    // Richtungen für Nachbarzellen (oben, rechts, unten, links)
    int directions[4][2] = {
        {-1, 0}, // oben
        {0, 1},  // rechts
        {1, 0},  // unten
        {0, -1}  // links
    };

    // Solange die Warteschlange nicht leer ist
    while (front < back && back <= (26*26)) {
        // Hole den nächsten Punkt aus der Warteschlange
        Point current = queue[front++];
        int x = current.x;
        int y = current.y;

        // Ändere die Farbe der aktuellen Zelle
        gameState.field[x][y] = gameState.currentColor;

        // Überprüfe alle Nachbarn
        for (int i = 0; i < 4; i++) {
            int newX = x + directions[i][0];
            int newY = y + directions[i][1];

            // Stelle sicher, dass die Nachbarn innerhalb des Feldes sind
            if (newX >= 0 && newX < gameState.fieldSize && newY >= 0 && newY < gameState.fieldSize) {
                // Prüfe, ob die Nachbarzelle die ursprüngliche Farbe hat
                if (gameState.field[newX][newY] == originalColor) {
                    // Füge die Nachbarzelle zur Warteschlange hinzu
                    gameState.field[newX][newY] = gameState.currentColor;
                    queue[back++] = (Point) {newX, newY};
                }
            }
        }
    }
}

void setNumberOfMoves() {
    gameState.numberOfMoves = (int) gameState.fieldSize*0.2975*gameState.numberOfColors;
}

bool inField(int x, int y) {
    // Grenzen prüfen
    if (x < 0 || x >= 26 || y < 0 || y >= 26) {
        return false;
    } else {
        return true;
    }
}

bool isWithinRange(int x) {
    if (x < 0 || x >= 26) {
        return false;
    } else {
        return true;
    }
}

void increaseFieldSize() {
    if (gameState.fieldSize == 26) { //TODO: Was ist die Minimale Grösse?
        gameState.fieldSize = 8;
    } else {
        gameState.fieldSize++;
    }
}

void decreaseFieldSize() {
    if (gameState.fieldSize == 8) { //TODO: Was ist die Minimale Grösse?
        gameState.fieldSize = 26;
    } else {
        gameState.fieldSize--;
    }
}

void increaseNumberOfColors() {
    if (gameState.numberOfColors == 8) { //TODO: Was ist die Minimale Grösse?
        gameState.numberOfColors = 3;
    } else {
        gameState.numberOfColors++;
    }
}

void decreaseNumberOfColors() {
    if (gameState.numberOfColors == 3) { //TODO: Was ist die Minimale Grösse?
        gameState.numberOfColors = 8;
    } else {
        gameState.numberOfColors--;
    }
}

bool is_won() { // TODO: Array dynamisch machen.
    int color = gameState.field[0][0];
    for (int i = 0; i < gameState.fieldSize; i++) {
        for (int j = 0; j < gameState.fieldSize; j++) {
            if (gameState.field[i][j] != color) {
                return false; // Noch nicht gewonnen
            }
        }
    }
    return true; // Gewonnen
}

void selectionModeSize() {
    gameState.screen = SIZE_SELECTION;
    UpdateLayout();
    DrawScreen();
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

void selectionModeColors() {
    gameState.screen = COLOR_SELECTION;
    UpdateLayout();
    DrawScreen();
    while (!ShouldEndGame()) {
        // TODO implement logic here (this code was only for testing the visuals)
        if (ButtonHasBeenPressed(UP)) {
            increaseNumberOfColors();
            UpdateLayout();
        }
        if (ButtonHasBeenPressed(DOWN)) {
            decreaseNumberOfColors();
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

void titleScreen() {
    gameState.screen = TITLE;
    UpdateLayout();
    DrawScreen();
    while(!ShouldEndGame()) {
        if (ButtonHasBeenPressed(ENTER)) {
            return;
        }
        DrawScreen();
    }
}

void gameMode() {
    gameState.currentColor = 0;
    gameState.screen = GAME;
    UpdateLayout();
    DrawScreen();
    while(!ShouldEndGame()) {
        if (ButtonHasBeenPressed(UP)) {
            selectColorInGameUP();
        }

        if (ButtonHasBeenPressed(DOWN)) {
            selectColorInGameDOWN();
        }

        if (ButtonHasBeenPressed(ENTER)) { // TODO
            int originalColor = gameState.field[0][0]; // Die ursprüngliche Farbe oben links
            if (originalColor != gameState.currentColor) {
                floodFillIterative();
                if (is_won()) {
                    wonScreen();
                    return;
                }
                
                gameState.numberOfMoves--;
                if (gameState.numberOfMoves == 0) {
                    lostScreen();
                    return;
                }
            }
        }
        DrawScreen();
    }
}

void selectColorInGameUP() {
    if (chosenNumberOfColor - 1 == gameState.currentColor) { //TODO: Was ist die Minimale Grösse?
        gameState.currentColor = 0;
    } else {
        gameState.currentColor++;
    }

}

void selectColorInGameDOWN() {
    if (gameState.currentColor == 0) { //TODO: Was ist die Minimale Grösse?
        gameState.currentColor = chosenNumberOfColor - 1;
    } else {
        gameState.currentColor--;
    }
}

void wonScreen() {
    gameState.screen = WON;
    UpdateLayout();
    DrawScreen();
    while (!ShouldEndGame()) {
        if (ButtonHasBeenPressed(ENTER)) { // TODO
            return;
        }
        DrawScreen();
    }
}

void lostScreen() {
    gameState.screen = LOST;
    UpdateLayout();
    DrawScreen();
    while (!ShouldEndGame()) {
        if (ButtonHasBeenPressed(ENTER)) { // TODO
            return;
        }
        DrawScreen();
    }
}

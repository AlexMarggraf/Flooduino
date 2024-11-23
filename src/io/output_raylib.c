// this file implements the functions declared in output.h using raylib

// todos:
// TODO differentiate the visuals of color and size selection screen
// TODO implement the static screens (title, won, lost)

#include "raylib.h"
#include "../common.h"
#include "output.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


static int screenWidth = 650;
static int screenHeight = 650;
static const Color colors[8] = {RED, GREEN, BLUE, YELLOW, ORANGE, PURPLE, LIME, BROWN};
static Color screen[64][64];

static const int wantedMargin = 10;
static int actualMargin = -1;
static int fieldBlockSize = -1;
static int fieldOffset = -1;
static const int colSelectionBlockSize = 2;

// rendering of the different screen states
void RenderTitleScreen(void);
void RenderColorSelectionScreen(void);
void RenderSizeSelectionScreen(void);
void RenderGameScreen(void);
void RenderWonScreen(void);
void RenderLostScreen(void);

// show/reset the rendered screen
void ShowScreen(void);
void ResetScreen(void);

// debugging
void DrawDebugInformation(void);

// utility render functions
void RenderField(bool);
void RenderColorSelection(int);
void RenderSelectedColor(void);
void RenderNumberOfMoves(void);

// public functions
//--------------------------------------------------------------------------------------
void DrawScreen(void) {
    BeginDrawing();

    ClearBackground(BLACK);

    ResetScreen();

    switch(gameState.screen)
    {
        case TITLE: RenderTitleScreen(); break;
        case COLOR_SELECTION: RenderColorSelectionScreen(); break;
        case SIZE_SELECTION: RenderSizeSelectionScreen(); break;
        case GAME: RenderGameScreen(); break;
        case WON: RenderWonScreen(); break;
        case LOST: RenderLostScreen(); break;
        default: break;
    }

    ShowScreen();

    DrawDebugInformation();

    DrawFPS(screenWidth - 30, screenHeight - 30);

    EndDrawing();
}

void InitScreen(const char* name) {

    // Initialization
    InitWindow(screenWidth, screenHeight, name);

    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second

    // TODO change this
}

void UpdateLayout(void) {
    fieldBlockSize = (64 - wantedMargin)/gameState.fieldSize; 
    actualMargin = 64 - fieldBlockSize*gameState.fieldSize;
    fieldOffset = 64 - gameState.fieldSize*fieldBlockSize;
}

void TerminateScreen(void) {
    CloseWindow();
}

bool ShouldEndGame(void) {
    return WindowShouldClose();
}
//--------------------------------------------------------------------------------------

void RenderTitleScreen(void) {
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            screen[y][x] = BLUE;
        }
    }
}

void RenderColorSelectionScreen(void){
    RenderColorSelection(-1);
    RenderField(false);
}

void RenderSizeSelectionScreen(void){
    RenderColorSelection(-1);
    RenderField(false);
}


void RenderGameScreen(void) {

    RenderField(true);

    RenderColorSelection(gameState.currentColor);

    // draw the selected color at the top left of the field
    for (int y = 0; y < fieldOffset; y++) {
        for (int x = 0; x < fieldOffset; x++) {
            screen[y][x] = colors[gameState.currentColor];
        }
    }

    // draw the moves
    int horizontalSpace = (64 - fieldOffset) / 2 * 2;
    for (int i = 0; i < gameState.numberOfMoves; i++) {
        screen[i/(horizontalSpace/2) * 2][i * 2 % horizontalSpace + fieldOffset + 1] = GRAY;
    }
}

void RenderWonScreen(void){
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            screen[y][x] = GREEN;
        }
    }
}

void RenderLostScreen(void){
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            screen[y][x] = RED;
        }
    }
}

void ShowScreen(void) {
    int offset = 10;
    int pixelSize = 10;
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            DrawCircle(x * pixelSize + offset, y * pixelSize + offset, pixelSize/2, screen[y][x]);
        }
    }
}

void ResetScreen(void) {
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            screen[y][x] = BLANK;
        }
    }
}

void DrawDebugInformation(void) {
    char a[150];
    int len = sprintf(&a, "screen: %d\nnumberOfMoves: %d\ncurrentColor: %d\nfieldSize: %d\nnumberOfColors: %d", gameState.screen, gameState.numberOfMoves, gameState.currentColor, gameState.fieldSize, gameState.numberOfColors);
    DrawText(a, 20,150, 5, WHITE);
}

void RenderField(bool colorful) {
    if (colorful) {
        // draw field
        for (int y = 0; y < gameState.fieldSize*fieldBlockSize; y++) {
            for (int x = 0; x < gameState.fieldSize*fieldBlockSize; x++) {
                screen[y+fieldOffset][x+fieldOffset] = colors[gameState.field[y/fieldBlockSize][x/fieldBlockSize]];
            }
        }
    } else {
        for (int y = 0; y < gameState.fieldSize*fieldBlockSize; y++) {
            for (int x = 0; x < gameState.fieldSize*fieldBlockSize; x++) {
                screen[y+fieldOffset][x+fieldOffset] = (y/fieldBlockSize+x/fieldBlockSize)%2 == 0 ? WHITE : BLANK;
            }
        }
    }
}

// render color selection bar, with a highlighted color (selectedColor) 
// if selecedColor is negative, only the color selection bar is rendered
void RenderColorSelection(int selectedColor) {
    // draw color selection bar
    // TODO this could be optimized
    for (int c = 0; c < gameState.numberOfColors; c++) {
        int colBlockOffset = 64 - (colSelectionBlockSize + 1)*(c + 1);
        Color currCol = colors[c];
        for (int y = 0; y < colSelectionBlockSize; y++) {
            for (int x = 0; x < colSelectionBlockSize; x++) {
                screen[y+colBlockOffset][x+1] = currCol;
            }
        }
    }

    // draw highlight of currently selected color
    if (selectedColor < 0) {
        return;
    }
    int colHighlightOffset = 64 - (colSelectionBlockSize + 1) * (selectedColor + 1) - 1;
    for (int y = 0; y < colSelectionBlockSize + 2; y++) {
        for (int x = 0; x < colSelectionBlockSize + 2; x++) {
            if (x == 0 || x == colSelectionBlockSize + 1 || y == 0 || y == colSelectionBlockSize + 1) {
                screen[y+colHighlightOffset][x] = WHITE;
            }
        }
    }
}
void RenderSelectedColor(void) {

}
void RenderNumberOfMoves(void) {

}
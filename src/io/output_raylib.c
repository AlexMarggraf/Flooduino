/** 
 * this file implements the functions declared in output.h using raylib. 
*/

// todos (p-vf):
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
static Color colors[8] = {{ 230, 41, 55, 255 }, 
    { 0, 228, 48, 255 },
    { 0, 121, 241, 255 },
    { 253, 249, 0, 255 },
    { 255, 161, 0, 255 },
    { 200, 122, 255, 255 },
    { 0, 158, 47, 255 },
    { 127, 106, 79, 255 }};
static Color ledScreen[64][64];

static const int wantedMargin = 10;
static int actualMargin = -1;
static int fieldBlockSize = -1;
static int fieldOffset = -1;
static const int colSelectionBlockSize = 2;

// rendering of the different screen states to the color-array ledScreen
void RenderTitleLedScreen(void);
void RenderColorSelectionLedScreen(void);
void RenderSizeSelectionLedScreen(void);
void RenderGameLedScreen(void);
void RenderWonLedScreen(void);
void RenderLostLedScreen(void);

// show/reset the contents of ledScreen
void ShowLedScreen(void);
void ResetLedScreen(void);

// debugging
void DrawDebugInformation(void);

// utility render functions (for rendering layout onto the ledScreen)
void RenderField(bool colorful);
void RenderColorSelection(int selectedColor);
void RenderSelectedColor(void);
void RenderNumberOfMoves(void);

// public functions
//--------------------------------------------------------------------------------------
void DrawScreen(void) {
    BeginDrawing();

    ClearBackground(BLACK);

    ResetLedScreen();

    switch(gameState.screen)
    {
        case TITLE: RenderTitleLedScreen(); break;
        case COLOR_SELECTION: RenderColorSelectionLedScreen(); break;
        case SIZE_SELECTION: RenderSizeSelectionLedScreen(); break;
        case GAME: RenderGameLedScreen(); break;
        case WON: RenderWonLedScreen(); break;
        case LOST: RenderLostLedScreen(); break;
        default: break;
    }

    ShowLedScreen();

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

void RenderTitleLedScreen(void) {
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            ledScreen[y][x] = BLUE;
        }
    }
}

void RenderColorSelectionLedScreen(void){
    RenderColorSelection(-1);
    RenderField(false);
}

void RenderSizeSelectionLedScreen(void){
    RenderColorSelection(-1);
    RenderField(false);
}


void RenderGameLedScreen(void) {

    RenderField(true);

    RenderColorSelection(gameState.currentColor);

    // draw the selected color at the top left of the field
    for (int y = 0; y < fieldOffset; y++) {
        for (int x = 0; x < fieldOffset; x++) {
            ledScreen[y][x] = colors[gameState.currentColor];
        }
    }

    RenderNumberOfMoves();
}

void RenderWonLedScreen(void){
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            ledScreen[y][x] = GREEN;
        }
    }
}

void RenderLostLedScreen(void){
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            ledScreen[y][x] = RED;
        }
    }
}

void ShowLedScreen(void) {
    int offset = 10;
    int pixelSize = 10;
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            DrawCircle(x * pixelSize + offset, y * pixelSize + offset, pixelSize/2, ledScreen[y][x]);
        }
    }
}

/**
 * 
 */
void ResetLedScreen(void) {
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            ledScreen[y][x] = BLANK;
        }
    }
}

/**
 * Draws all the numbers of gameState (except the field array) to the screen. 
 */
void DrawDebugInformation(void) {
    char a[150];
    int len = sprintf(&a, "screen: %d\nnumberOfMoves: %d\ncurrentColor: %d\nfieldSize: %d\nnumberOfColors: %d", gameState.screen, gameState.numberOfMoves, gameState.currentColor, gameState.fieldSize, gameState.numberOfColors);
    DrawText(a, 20,150, 5, WHITE);
}

/**
 * Renders the field in the bottom right corner. 
 * @param[in] colorful Specifies if the field should be rendered colorful. 
 *                     If true, the contents of gameState.field are rendered, otherwise a checkerboard pattern is rendered.
 */
void RenderField(bool colorful) {
    if (colorful) {
        // draw field
        for (int y = 0; y < gameState.fieldSize*fieldBlockSize; y++) {
            for (int x = 0; x < gameState.fieldSize*fieldBlockSize; x++) {
                ledScreen[y+fieldOffset][x+fieldOffset] = colors[gameState.field[y/fieldBlockSize][x/fieldBlockSize]];
            }
        }
    } else {
        for (int y = 0; y < gameState.fieldSize*fieldBlockSize; y++) {
            for (int x = 0; x < gameState.fieldSize*fieldBlockSize; x++) {
                ledScreen[y+fieldOffset][x+fieldOffset] = (y/fieldBlockSize+x/fieldBlockSize)%2 == 0 ? WHITE : BLANK;
            }
        }
    }
}

/** 
 * Render color selection bar, with the in selectedColor specified color (if non-negative).
 * If selecedColor is negative, only the color selection bar is rendered, without highlight. 
 */
void RenderColorSelection(int selectedColor) {
    // draw color selection bar
    // TODO this could be optimized (only two nested for-loops)
    for (int c = 0; c < gameState.numberOfColors; c++) {
        int colBlockOffset = 64 - (colSelectionBlockSize + 1)*(c + 1);
        Color currCol = colors[c];
        for (int y = 0; y < colSelectionBlockSize; y++) {
            for (int x = 0; x < colSelectionBlockSize; x++) {
                ledScreen[y+colBlockOffset][x+1] = currCol;
            }
        }
    }
    
    if (selectedColor < 0) {
        return;
    }

    // draw highlight of currently selected color
    int colHighlightOffset = 64 - (colSelectionBlockSize + 1) * (selectedColor + 1) - 1;
    for (int y = 0; y < colSelectionBlockSize + 2; y++) {
        for (int x = 0; x < colSelectionBlockSize + 2; x++) {
            if (x == 0 || x == colSelectionBlockSize + 1 || y == 0 || y == colSelectionBlockSize + 1) {
                ledScreen[y+colHighlightOffset][x] = WHITE;
            }
        }
    }
}

void RenderSelectedColor(void) {

}

/**
 * Renders the number of moves done until now, in the upper right part of the screen as grey dots
 */
void RenderNumberOfMoves(void) {
    // draw the moves
    int horizontalSpace = (64 - fieldOffset) / 2 * 2;
    for (int i = 0; i < gameState.numberOfMoves; i++) {
        ledScreen[i/(horizontalSpace/2) * 2][i * 2 % horizontalSpace + fieldOffset + 1] = GRAY;
    }
}
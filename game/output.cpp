/** 
 * this file implements the functions declared in output.h for the 64 by 64 led-matrix display using the arduino. 
*/

#include "RGBmatrixPanel.h"

#include "bit_bmp.h"
#include "image_bitmaps.h"
#include "fonts.h"
#include <string.h>

#include "common.h"
#include "output.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef uint16_t Color;

static int screenWidth = 650;
static int screenHeight = 650;
static Color colors[8];  // initialized later (in InitScreen)


// ---------------- EYEBALL PROTECTION ----------------
const int darkness = 1;  // value range: 1 (full brightness) to 5 (less bright, nicer on the eyes over long periods)
// scale values down
constexpr int adj(int v) {
  return v > 0 ? (v - 1) / darkness + 1 : 0;
}
// apply them on each call to Color333. A bit hacky.
#define Color333(a, b, c) Color333(adj((a)), adj((b)), adj((c)))
// ---------------- ------------------ ----------------


#define WHITE matrix.Color333(7, 7, 7)
#define BLACK matrix.Color333(0, 0, 0)



//#define CLK  8   // USE THIS ON ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
#define CLK 11  // USE THIS ON ARDUINO MEGA
#define OE 9
#define LAT 10
#define A A0
#define B A1
#define C A2
#define D A3
#define E A4

RGBmatrixPanel matrix(A, B, C, D, E, CLK, LAT, OE, false, 64);


static const int wantedFieldOffset = 10;
static int fieldBlockSize = -1;
static int fieldOffset = -1;
static const int colSelectionBlockSize = 3;

static GameState prevGameState = {};

// rendering of the different screen states to the color-array ledScreen
void RenderTitleLedScreen(void);
void RenderColorSelectionLedScreen(void);
void RenderSizeSelectionLedScreen(void);
void RenderGameLedScreen(void);
void RenderWonLedScreen(void);
void RenderLostLedScreen(void);

// show/reset the contents of ledScreen
//void ShowLedScreen(void); // raylib specific
void ResetLedScreen(void);

// utility render functions (for rendering layout onto the ledScreen)
void RenderField(bool colorful, bool highlighted);
void RenderColorSelection(int selectedColor, bool highlighted);
void UpdateColorSelectionInGame(int prevSelectedColor, int selectedColor);
void UpdateNumberOfColorsSelection(int prevNumberOfColors, int numberOfColors);
void RenderNumberOfMoves(void);

// display utility functions
void RenderRectFilled(Point p, int w, int h, Color c);
void RenderRectOutline(Point p, int w, int h, Color c);
void RenderDot(Point p, Color c);
void FillScreen(Color c);
// functions to dynamically update screen
bool GameStateUnchanged();


// public functions
//--------------------------------------------------------------------------------------
void DrawScreen(void) {
  if (GameStateUnchanged()) {
    prevGameState = gameState;
    return;
  }

  switch (gameState.screen) {
    case TITLE: RenderTitleLedScreen(); break;
    case COLOR_SELECTION: RenderColorSelectionLedScreen(); break;
    case SIZE_SELECTION: RenderSizeSelectionLedScreen(); break;
    case GAME: RenderGameLedScreen(); break;
    case WON: RenderWonLedScreen(); break;
    case LOST: RenderLostLedScreen(); break;
    default: break;
  }

  prevGameState = gameState;
}

void InitScreen(void) {
  colors[0] = matrix.Color333(7, 0, 0);  // Pure Red
  colors[1] = matrix.Color333(7, 7, 0);  // Pure Yellow
  colors[2] = matrix.Color333(0, 5, 7);  // Light Blue
  colors[3] = matrix.Color333(0, 3, 0);  // Medium Green
  colors[4] = matrix.Color333(0, 0, 4);  // Dark Blue
  colors[5] = matrix.Color333(7, 3, 3);  // Skin Color
  colors[6] = matrix.Color333(2, 0, 1);  // Purple
  colors[7] = matrix.Color333(1, 1, 2);  // dark Grey
  matrix.begin();
}

void UpdateLayout(void) {
  fieldBlockSize = (64 - (wantedFieldOffset + 1)) / gameState.fieldSize;
  fieldOffset = 64 - gameState.fieldSize * fieldBlockSize - 1;
}

//--------------------------------------------------------------------------------------

bool GameStateUnchanged() {
  GameState g1 = prevGameState;
  GameState g2 = gameState;
  // NOTE: the fields of the two gamestates are not checked because I don't know how to do that
  //   + it doesn't matter because everytime the field changes, another variable changes as well.
  return g1.screen == g2.screen && g1.fieldSize == g2.fieldSize && g1.numberOfColors == g2.numberOfColors && g1.numberOfMoves == g2.numberOfMoves && g1.currentColor == g2.currentColor && g1.maxNumberOfMoves == g2.maxNumberOfMoves;
}

bool onlyHighlightedColorChanged() {
  GameState g1 = prevGameState;
  GameState g2 = gameState;
  return g1.currentColor != g2.currentColor && g1.screen == g2.screen && g1.fieldSize == g2.fieldSize && g1.numberOfColors == g2.numberOfColors && g1.numberOfMoves == g2.numberOfMoves && g1.maxNumberOfMoves == g2.maxNumberOfMoves;
}

void RenderTitleLedScreen(void) {
  ResetLedScreen();

  matrix.display_image(0,0, titleScreen, 64, 64);
}

void RenderColorSelectionLedScreen(void) {
  if (prevGameState.screen != gameState.screen) { // game was not on color selection before => reset necessary
    ResetLedScreen();

    RenderColorSelection(-1, true);
    RenderField(false, false);
    return;
  }
  UpdateNumberOfColorsSelection(prevGameState.numberOfColors, gameState.numberOfColors);
}

void RenderSizeSelectionLedScreen(void) {
  ResetLedScreen();

  RenderColorSelection(-1, false);
  RenderField(false, true);
}


void RenderGameLedScreen(void) {

  if (onlyHighlightedColorChanged()) {
    UpdateColorSelectionInGame(prevGameState.currentColor, gameState.currentColor);
    return;
  }

  ResetLedScreen();

  RenderField(true, false);

  RenderColorSelection(gameState.currentColor, false);

  RenderNumberOfMoves();
}

void RenderWonLedScreen(void) {
  ResetLedScreen();

  matrix.display_image(0, 0, wonScreen, 64, 64);
}

void RenderLostLedScreen(void) {
  ResetLedScreen();

  matrix.display_image(0, 0 , lostScreen, 64, 64);
}

void ResetLedScreen(void) {
  matrix.fillRect(0, 0, matrix.width(), matrix.height(), BLACK);
}

/**
 * Renders the field in the bottom right corner. 
 * @param[in] colorful Specifies if the field should be rendered colorful. 
 *                     If true, the contents of gameState.field are rendered, otherwise a checkerboard pattern is rendered.
 */
void RenderField(bool colorful, bool highlighted) {

  if (colorful) {
    // draw field
    for (int i = 0; i < gameState.fieldSize; i++) {
      for (int j = 0; j < gameState.fieldSize; j++) {
        RenderRectFilled((Point){ j * fieldBlockSize + fieldOffset, i * fieldBlockSize + fieldOffset }, fieldBlockSize, fieldBlockSize, colors[gameState.field[i][j]]);
      }
    }
  } else {
    for (int i = 0; i < gameState.fieldSize; i++) {
      for (int j = 0; j < gameState.fieldSize; j++) {
        Color col = (i + j) % 2 == 0 ? WHITE : BLACK;
        RenderRectFilled((Point){ j * fieldBlockSize + fieldOffset, i * fieldBlockSize + fieldOffset }, fieldBlockSize, fieldBlockSize, col);
      }
    }
  }

  if (highlighted) {
    int o = fieldOffset - 1;
    RenderRectOutline((Point){ o, o }, 64 - o, 64 - o, WHITE);
  }
}

/** 
 * Render color selection bar, with the in selectedColor specified color (if non-negative).
 * If selecedColor is negative, only the color selection bar is rendered, without highlight. 
 * If highlighted is true, the whole colorselection list is highlighted. 
 */
void RenderColorSelection(int selectedColor, bool highlighted) {
  // draw color selection bar
  for (int c = 0; c < gameState.numberOfColors; c++) {
    int colBlockOffset = 64 - (colSelectionBlockSize + 1) * (c + 1);
    Color currCol = colors[c];
    RenderRectFilled((Point){ 1, colBlockOffset }, colSelectionBlockSize, colSelectionBlockSize, currCol);
  }

  if (highlighted) {
    int height = (colSelectionBlockSize + 1) * gameState.numberOfColors + 1;
    int offset = 64 - height;
    RenderRectOutline((Point){ 0, offset }, colSelectionBlockSize + 2, height, WHITE);
  }

  if (selectedColor < 0) {
    return;
  }

  // draw the selected color at the top left of the field
  RenderRectFilled((Point){ 1, 1 }, fieldOffset - 1, fieldOffset - 1, colors[gameState.currentColor]);


  // draw highlight of currently selected color
  int colHighlightOffset = 64 - (colSelectionBlockSize + 1) * (selectedColor + 1) - 1;
  RenderRectOutline((Point){ 0, colHighlightOffset }, colSelectionBlockSize + 2, colSelectionBlockSize + 2, WHITE);
}

void UpdateColorSelectionInGame(int prevSelectedColor, int selectedColor) {
  if (prevSelectedColor == selectedColor) {
    return;
  }

  // draw the selected color at the top left of the field
  RenderRectFilled((Point){ 1, 1 }, fieldOffset - 1, fieldOffset - 1, colors[gameState.currentColor]);

  // delete highlight of previously selected color
  int colHighlightOffset = 64 - (colSelectionBlockSize + 1) * (prevSelectedColor + 1) - 1;
  RenderRectOutline((Point){ 0, colHighlightOffset }, colSelectionBlockSize + 2, colSelectionBlockSize + 2, BLACK);

  // draw highlight of currently selected color
  colHighlightOffset = 64 - (colSelectionBlockSize + 1) * (selectedColor + 1) - 1;
  RenderRectOutline((Point){ 0, colHighlightOffset }, colSelectionBlockSize + 2, colSelectionBlockSize + 2, WHITE);
}

void UpdateNumberOfColorsSelection(int prevNumberOfColors, int numberOfColors) {
  if (prevNumberOfColors == numberOfColors) {
    return;
  }
  
  int prevHeight = (colSelectionBlockSize + 1) * prevNumberOfColors + 1;
  int prevOffset = 64 - prevHeight;

  RenderRectFilled((Point){ 0, prevOffset }, colSelectionBlockSize + 2, prevHeight, BLACK);

  for (int c = 0; c < numberOfColors; c++) {
    int colBlockOffset = 64 - (colSelectionBlockSize + 1) * (c + 1);
    Color currCol = colors[c];
    RenderRectFilled((Point){ 1, colBlockOffset }, colSelectionBlockSize, colSelectionBlockSize, currCol);
  }

  int height = (colSelectionBlockSize + 1) * numberOfColors + 1;
  int offset = 64 - height;
  RenderRectOutline((Point){ 0, offset }, colSelectionBlockSize + 2, height, WHITE);
}

/**
 * Renders the number of moves done until now, in the upper right part of the screen as grey dots
 */
void RenderNumberOfMoves(void) {
  // draw the moves
  /*int horizontalSpace = (64 - fieldOffset) / 2 * 2;
  for (int i = 0; i < gameState.numberOfMoves; i++) {
    RenderDot((Point){ i * 2 % horizontalSpace + fieldOffset + 1, i / (horizontalSpace / 2) * 2 + 1 }, WHITE);
  }*/
  
  char movesStringBuffer[8];
  
  int numOfChars = sprintf(movesStringBuffer, "%d/%d", gameState.numberOfMoves, gameState.maxNumberOfMoves);

  Color noMovesColor = gameState.numberOfMoves <= gameState.maxNumberOfMoves ? matrix.Color333(7,7,7) : matrix.Color333(7,0,0);

  for (int i = 0; i < numOfChars; i++) {
    if (movesStringBuffer[i] == '/') {
      noMovesColor = matrix.Color333(7,7,7);
    }
    matrix.drawChar((64 - 3 - 6*numOfChars) + i * 6, 4, movesStringBuffer[i], noMovesColor, matrix.Color333(0,0,0), 1);
  }
}


/**
 * Renders a filled rectangle that starts at the top left point p and extends with width w to the right and with height h down. 
 */
void RenderRectFilled(Point p, int w, int h, Color c) {
  matrix.fillRect(p.x, p.y, w, h, c);
}

/**
 * Renders a filled rectangle that starts at the top left point p and extends with width w to the right and with height h down. 
 */
void RenderRectOutline(Point p, int w, int h, Color c) {
  matrix.drawRect(p.x, p.y, w, h, c);
}

/**
 * Draws a dot at the specified location with the specified color. 
 */
void RenderDot(Point p, Color c) {
  matrix.drawPixel(p.x, p.y, c);
}

/**
 * Fill the LED-screen with one color
 */
void FillScreen(Color c) {
  matrix.fillRect(0, 0, matrix.width(), matrix.height(), c);
}

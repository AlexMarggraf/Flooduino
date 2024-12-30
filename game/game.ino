#include "common.h"  // NOTE: Declares global (extern) variable and types
#include "input.h"
#include "output.h"
#include <stdlib.h>
#include <stdbool.h>
#include "DFRobotDFPlayerMini.h"

//----------------------------------------------------------------------------------
// Shared Variable Definition (global)
// NOTE: This variable is shared between modules through common.h
//----------------------------------------------------------------------------------
GameState gameState = { 0 };

// the seed for the random colors in the game-field
#define SEED_SOURCE (analogRead(A8) + analogRead(A9) + analogRead(A10) + analogRead(A11) + analogRead(A12) + analogRead(A13))

// function declarations
void SetGamefield(void);
void floodFillIterative();
void setNumberOfMoves(void);  //TODO: implement this function
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

// DFPlayer shenanigans
// file 1: Title screen
// file 2: Size/number of colors seleciton
// file 3: Game music
// file 4: won screen
// file 5: lost screen
void nextSong(void);
void resetDFPlayer(void); // this method is not implemented in our design
void increaseVolume(void);
#define DFPlayerBusyPin 5
#define DFPlayerNextSongPin 4
#define numberOfSongs 5

// these are field sizes that make sense visually
int fieldSizes[5] = { 8, 10, 13, 17, 26 };
int fieldSizeIndex = 0;

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  
  pinMode(DFPlayerBusyPin, INPUT_PULLUP);
  pinMode(DFPlayerNextSongPin, OUTPUT);
  digitalWrite(DFPlayerNextSongPin, HIGH);
  delay(100);
  loopSong();

  setupInput();
  InitScreen();
  UpdateLayout();
}

void loop() {
  // main game loop
  SetInitialState();
  titleScreen();
  gameState.numberOfColors = 3;
  nextSong();
  selectionModeSize();
  gameState.currentColor = 2;
  selectionModeColors();
  nextSong();
  SetGamefield();
  gameMode();
  nextSong();
}

void SetGamefield() {
  srand(SEED_SOURCE);
  for (int i = 0; i < 26; i++) {
    for (int j = 0; j < 26; j++) {
      gameState.field[i][j] = rand() % gameState.numberOfColors;
    }
  }
}

void SetInitialState() {
  gameState.screen = GAME;
  gameState.fieldSize = fieldSizes[fieldSizeIndex];
  gameState.numberOfColors = 8;
  gameState.numberOfMoves = 0;
  gameState.currentColor = 1;
  srand(SEED_SOURCE);
  for (int i = 0; i < 26; i++) {
    for (int j = 0; j < 26; j++) {
      gameState.field[i][j] = rand() % 8;
    }
  }
}

// Funktion, um den Flood-It-Algorithmus auszuführen
void floodFillIterative() {
  int originalColor = gameState.field[0][0];  // Die ursprüngliche Farbe oben links

  // Warteschlange für Punkte (Koordinaten), die noch verarbeitet werden müssen
  Point queue[26 * 26];
  int front = 0, back = 0;

  // Füge den Startpunkt (oben links) zur Warteschlange hinzu
  queue[back++] = (Point){ 0, 0 };

  // Richtungen für Nachbarzellen (oben, rechts, unten, links)
  int directions[4][2] = {
    { -1, 0 },  // oben
    { 0, 1 },   // rechts
    { 1, 0 },   // unten
    { 0, -1 }   // links
  };

  // Solange die Warteschlange nicht leer ist
  while (front < back && back <= (26 * 26)) {
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
          queue[back++] = (Point){ newX, newY };
        }
      }
    }
  }
}

/**
 * This method calculates the number of moves can make, based on the number of colors and the size of the field. 
 */
void setNumberOfMoves() {
  gameState.maxNumberOfMoves = (int)gameState.fieldSize * 0.2975 * gameState.numberOfColors;
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
  if (fieldSizeIndex == 4) {  //TODO: Was ist die Minimale Grösse?
    fieldSizeIndex = 0;
  } else {
    fieldSizeIndex++;
  }
  gameState.fieldSize = fieldSizes[fieldSizeIndex];
}

void decreaseFieldSize() {
  if (fieldSizeIndex == 0) {  //TODO: Was ist die Minimale Grösse?
    fieldSizeIndex = 4;
  } else {
    fieldSizeIndex--;
  }  
  gameState.fieldSize = fieldSizes[fieldSizeIndex];
}

void increaseNumberOfColors() {
  if (gameState.numberOfColors == 8) {  //TODO: Was ist die Minimale Grösse?
    gameState.numberOfColors = 3;
  } else {
    gameState.numberOfColors++;
  }
}

void decreaseNumberOfColors() {
  if (gameState.numberOfColors == 3) {  //TODO: Was ist die Minimale Grösse?
    gameState.numberOfColors = 8;
  } else {
    gameState.numberOfColors--;
  }
}

bool is_won() {  // TODO: Array dynamisch machen.
  int color = gameState.field[0][0];
  for (int i = 0; i < gameState.fieldSize; i++) {
    for (int j = 0; j < gameState.fieldSize; j++) {
      if (gameState.field[i][j] != color) {
        return false;  // Noch nicht gewonnen
      }
    }
  }
  return true;  // Gewonnen
}

void selectionModeSize() {
  gameState.screen = SIZE_SELECTION;
  UpdateLayout();
  DrawScreen();
  while (true) {
    loopSong();
    // TODO implement logic here (this code was only for testing the visuals)
    if (buttonHasBeenPressed(UP)) {
      increaseFieldSize();
      UpdateLayout();
    }
    if (buttonHasBeenPressed(DOWN)) {
      decreaseFieldSize();
      UpdateLayout();
    }
    if (buttonHasBeenPressed(ENTER)) {
      setNumberOfMoves();
      UpdateLayout();
      //DrawScreen();
      return;
    }
    DrawScreen();
  }
}

void selectionModeColors() {
  gameState.screen = COLOR_SELECTION;
  UpdateLayout();
  DrawScreen();
  while (true) {
    loopSong();
    // TODO implement logic here (this code was only for testing the visuals)
    if (buttonHasBeenPressed(UP)) {
      increaseNumberOfColors();
      UpdateLayout();
    }
    if (buttonHasBeenPressed(DOWN)) {
      decreaseNumberOfColors();
      UpdateLayout();
    }
    if (buttonHasBeenPressed(ENTER)) {
      setNumberOfMoves();
      UpdateLayout();
      //DrawScreen();
      return;
    }
    DrawScreen();
  }
}

void titleScreen() {
  gameState.screen = TITLE;
  UpdateLayout();
  DrawScreen();
  while (true) {
    loopSong();
    if (buttonHasBeenPressed(ENTER)) {
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
  while (true) {
    loopSong();
    if (buttonHasBeenPressed(UP)) {
      selectColorInGameUP();
    }

    if (buttonHasBeenPressed(DOWN)) {
      selectColorInGameDOWN();
    }

    if (buttonHasBeenPressed(ENTER)) {            // TODO
      int originalColor = gameState.field[0][0];  // Die ursprüngliche Farbe oben links
      if (originalColor != gameState.currentColor) {
        floodFillIterative();
        gameState.numberOfMoves++;
        if (is_won() && gameState.numberOfMoves <= gameState.maxNumberOfMoves) {
          nextSong();
          wonScreen();
          nextSong();
          return;
        }
        if (is_won() && gameState.numberOfMoves > gameState.maxNumberOfMoves) {
          nextSong();
          nextSong();
          lostScreen();
          return;
        }
        
      }
    }
    DrawScreen();
  }
}

void selectColorInGameUP() {
  if (gameState.numberOfColors - 1 == gameState.currentColor) {  //TODO: Was ist die Minimale Grösse?
    gameState.currentColor = 0;
  } else {
    gameState.currentColor++;
  }
}

void selectColorInGameDOWN() {
  if (gameState.currentColor == 0) {  //TODO: Was ist die Minimale Grösse?
    gameState.currentColor = gameState.numberOfColors - 1;
  } else {
    gameState.currentColor--;
  }
}

void wonScreen() {
  gameState.screen = WON;
  UpdateLayout();
  DrawScreen();
  while (true) {
    loopSong();
    if (buttonHasBeenPressed(ENTER)) {  // TODO
      return;
    }
    DrawScreen();
  }
}

void lostScreen() {
  gameState.screen = LOST;
  UpdateLayout();
  DrawScreen();
  while (true) {
    loopSong();
    if (buttonHasBeenPressed(ENTER)) {  // TODO
      return;
    }
    DrawScreen();
  }
}


void increaseVolume() {
  digitalWrite(DFPlayerNextSongPin, LOW);
  delay(10000);
  digitalWrite(DFPlayerNextSongPin, HIGH);
  delay(100);
  //digitalWrite(DFPlayerNextSongPin, HIGH);
}

void nextSong() {
  digitalWrite(DFPlayerNextSongPin, HIGH);
  delay(100);
  digitalWrite(DFPlayerNextSongPin, LOW);
  delay(100);
  //digitalWrite(DFPlayerNextSongPin, HIGH);
}

void loopSong() {
  if (digitalRead(DFPlayerBusyPin) == 1) {
    for (int i = 0; i < numberOfSongs; i++) {
      nextSong();
    }
  }
}
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



#define SEED_SOURCE analogRead(A8)

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


int numberOfMoves;
int fieldSizeCounter;
int** gameBoard;
int chosenNumberOfColor;
int maxSize;



#define FPSerial Serial1

DFRobotDFPlayerMini myDFPlayer;

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
void setup() {
  FPSerial.begin(9600);

  //Serial.begin(115200);

  /*Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
*/
  if (!myDFPlayer.begin(FPSerial, /*isACK = */ true, /*doReset = */ true)) {  //Use serial to communicate with mp3.
    /*Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));*/
    while (true) {
      delay(0);  // Code to compatible with ESP8266 watch dog.
    }
  }
  //Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(20);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);     //Play the first mp3

  FPSerial.setTimeout(500);
  delay(550);


  setupInput();
  SetInitialState();
  InitScreen("Prototype Flooduino");
  UpdateLayout();

}

void loop() {
  // main game loop

  titleScreen();
  gameState.numberOfColors = 3;
  selectionModeSize();
  gameState.currentColor = 2;
  selectionModeColors();
  chosenNumberOfColor = gameState.numberOfColors;
  SetGamefield();
  gameMode();
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
  gameState.fieldSize = 10;
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

void setNumberOfMoves() {
  gameState.numberOfMoves = (int)gameState.fieldSize * 0.2975 * gameState.numberOfColors;
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
  if (gameState.fieldSize == 26) {  //TODO: Was ist die Minimale Grösse?
    gameState.fieldSize = 8;
  } else {
    gameState.fieldSize++;
  }
}

void decreaseFieldSize() {
  if (gameState.fieldSize == 8) {  //TODO: Was ist die Minimale Grösse?
    gameState.fieldSize = 26;
  } else {
    gameState.fieldSize--;
  }
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
  while (!ShouldEndGame()) {
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
  while (!ShouldEndGame()) {
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
  while (!ShouldEndGame()) {
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
  if (chosenNumberOfColor - 1 == gameState.currentColor) {  //TODO: Was ist die Minimale Grösse?
    gameState.currentColor = 0;
  } else {
    gameState.currentColor++;
  }
}

void selectColorInGameDOWN() {
  if (gameState.currentColor == 0) {  //TODO: Was ist die Minimale Grösse?
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
  while (!ShouldEndGame()) {
    if (buttonHasBeenPressed(ENTER)) {  // TODO
      return;
    }
    DrawScreen();
  }
}
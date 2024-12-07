#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <curses.h>	
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define GAMEBOARDWIDTH 24
#define GAMEBOARDHEIGHT 13
#define MAPSCOUNT 3
#define ROADSCOUNT 6
#define ASSETWIDTH 4
#define ASSETHEIGHT 2

#define ROAD 1
#define GRASS 2
#define FINISH 3
#define FOOTER 4

typedef enum {
	PREP,
	START,
	WON,
	LOST,
	QUIT
}GameState;

typedef enum {
	CAR,
	TIME
}LostBy;

typedef struct {
	int x;
	int y;
} Coordinates;

typedef struct {
	Coordinates coordinates;
	int direction; // 1 is right, -1 is left
	int speedMultiplier;
	time_t lastMoved;
}Car;

typedef struct {
	int jumpCooldown;
	time_t lastJumpTime;
	Coordinates coordinates;
}Frog;

typedef struct {
	time_t startTime;
	int timeLimit;
	int timeLeft;
	int isRunning;
}Timer;

typedef struct {
	char blocadePrint[ASSETHEIGHT][ASSETWIDTH];
	char carPrint[ASSETHEIGHT][ASSETWIDTH];
	char frogPrint[ASSETHEIGHT][ASSETWIDTH];
	char roadPrint[ASSETHEIGHT][ASSETWIDTH];
	char grassPrint[ASSETHEIGHT][ASSETWIDTH];
}Assets;

typedef struct {
	GameState gameState;
	int difficultyLevel;
	int mapNumber;
	Timer mainTimer;
	Car cars[ROADSCOUNT];
	char obstacleCharacter;
	Coordinates finishCoords;
	char gameBoard[GAMEBOARDHEIGHT][GAMEBOARDWIDTH];
	Frog frog;
	Assets assets;
	time_t lastMove;
	LostBy lostBy;
}Game;



typedef struct {
	int prevLine;
	int carsNumber;
} RoadsTracker;



//Main.c
void initCurses();
void initGame(Game* game);
int checkForJumpCooldown(Frog* frog);
void activateColor(const int colorPair); //activates given colors
void deactivateColor(const int colorPair); //deactivates given colors
int canJump(Game game, const int userInput); //checks if the frog can make a jump
void renderFrogMovement(const int prevY, const int prevX, const int y, const int x, Game game);
void jump(const int userInput, Game* game); //used for frog movement
void checkForFinish(Game* game); //checks if player reached the finish
int main();

//GameState.c
void handleStartState(Game* game);
void handleWonState(Game* game);
void handleLostState(Game* game);
int prepareGameResources(Game* game);
void handlePrepState(Game* game);
void handleGameStates(Game* game);
void printMessage(const Game* game, const char* message);
void printBlinkingMessage(const int startY, const char* message);

//LoadSettings.c
int loadSettings(Game* game); //loads and applies settings from txt file

//LoadMap.c
int checkForStart(const char ch, const int x, const int y, Game* game); //checks if loaded field is a start field
void display(const char ch, const int y, const int x, Game* game, RoadsTracker* roadsTracker); //displays loaded char with appropriate color
int loadMap(const char mapName[], Game* game); //loads and display map from a txt file
void printFooter();

//MainTimer.c
double getCurrentTimeInMs();
void initTimer(Game* game);
void setTimeLimit(Game* game, const int timeLimit);
void startTimer(Game* game);
void stopTimer(Game* game);
void resetTimer(Game* game);
void updateTime(Game* game);
int getTimeLeft(const Game* game);
int isTimerRunning(const Game* game);
void printTimer(const Game* game);

//Cars.c
int chooseBetween(const int firstCoord, const int secondCoord); //chooses random coordinate between the two
void radomizeStartSite(Game* game, const int carId);
void moveCar(Game* game, const int carId);
void toggleCarDirection(Car* car);
int checkForColision(Game* game, const int carId);
void randomizeMultiplier(Game* game, const int carId);
void setUpCar(Game* game, const int carId);

//AssetsHandler.c
int loadAssets(Assets* printables); //Loads assets from files
void mapAsset(FILE* file, char printable[ASSETHEIGHT][ASSETWIDTH]); //Maps assets into arrays
void printBlocade(const int y, const int x, const Assets* printables);
void printCar(const int y, const int x, const Game* game, const int prevX, const int carId);
void printFrog(const int y, const int x, const Assets* printables);
void printRoad(const int y, const int x, const Assets* printables);
void printGrass(const int y, const int x, const Assets* printables);
void print(const int y, const int x, const char printable[ASSETHEIGHT][ASSETWIDTH]); //General printing function

#endif
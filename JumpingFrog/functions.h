#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <curses.h>	
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define GAMEBOARDWIDTH 24
#define GAMEBOARDHEIGHT 13
#define MAPSCOUNT 3
#define ROADSCOUNT 5
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
}Game;



typedef struct {
	int prevLine;
	int carsNumber;
} RoadsTracker;



//Main.c
void initCurses();
void initGame(Game* game);
int checkForJumpCooldown(Frog* frog);
void activateColor(int colorPair); //activates given colors
void deactivateColor(int colorPair); //deactivates given colors
bool canJump(Game game, int userInput); //checks if the frog can make a jump
void renderFrogMovement(int prevY, int prevX, int y, int x, Game game);
void jump(int userInput, Game* game); //used for frog movement
void checkForFinish(Game* game); //checks if player reached the finish
int main();

//GameState.c
void handleStartState(Game* game);
void handleWonState(Game* game);
void handleLostState(Game* game);
bool prepareGameResources(Game* game);
void handlePrepState(Game* game);
void handleGameStates(Game* game);

//LoadSettings.c
bool loadSettings(Game* game); //loads and applies settings from txt file

//LoadMap.c
bool checkForStart(char ch, int x, int y, Game* game); //checks if loaded field is a start field
void display(char ch, int y, int x, Game* game, RoadsTracker* roadsTracker); //displays loaded char with appropriate color
bool loadMap(char mapName[], Game* game); //loads and display map from a txt file
void printFooter();

//MainTimer.c
double getCurrentTimeInMs();
void initTimer(Game* game);
void setTimeLimit(Game* game, int timeLimit);
void startTimer(Game* game);
void stopTimer(Game* game);
void resetTimer(Game* game);
void updateTime(Game* game);
int getTimeLeft(Game* game);
int isTimerRunning(Game* game);
void printTimer(Game* game);

//Cars.c
int choose_random(int firstCoord, int secondCoord); //chooses random coordinate between the two
void radomizeStartSite(Game* game, int carId);
void moveCar(Game* game, int carId);
void toggleCarDirection(Car* car);
int checkForColision(Game* game, int carId);
void randomizeMultiplier(Game* game, int carId);
void setUpCar(Game* game, int carId);

//AssetsHandler.c
int loadAssets(Assets* printables); //Loads assets from files
void mapAsset(FILE* file, char printable[ASSETHEIGHT][ASSETWIDTH]); //Maps assets into arrays
void printBlocade(int y, int x, Assets* printables);
void printCar(int y, int x, Game* game, int prevX, int carId);
void printFrog(int y, int x, Assets* printables);
void printRoad(int y, int x, Assets* printables);
void printGrass(int y, int x, Assets* printables);
void print(int y, int x, char printable[ASSETHEIGHT][ASSETWIDTH]); //General printing function

#endif
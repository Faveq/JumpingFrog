#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <curses.h>	
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <stdlib.h>

#define GAMEBOARDWIDTH 24
#define GAMEBOARDHEIGHT 13
#define MAPSCOUNT 3
#define ROADSCOUNT 5

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
	GameState gameState;
	int difficultyLevel;
	int mapNumber;
	Timer mainTimer;
	Car cars[ROADSCOUNT];
	char obstacleCharacter;
	Coordinates finishCoords;
	char gameBoard[GAMEBOARDHEIGHT][GAMEBOARDWIDTH];
	Frog frog;
}Game;

typedef struct {
	int prevLine;
	int carsNumber;
} RoadsTracker;

//Main.c
void initCurses(); //initialize curses
void initGame(Game* game); //initialize game
void activateColor(int colorPair); //activates given colors
void deactivateColor(int colorPair); //deactivates given colors
bool canJump(Game game, int userInput); //checks if the frog can make a jump
void jump(int userInput, Game* game); //used for frog movement
void checkForFinish(Game* game); //checks if player reached the finish


//LoadSettings.c
bool loadSettings(Game* game); //loads and applies settings from txt file

//LoadMap.c
bool checkForStart(char ch, int x, int y, Game* game); //checks if loaded fileld is a start field
void display(char ch, int y, int x, Game* game); //displays loaded char with appropriate color
bool loadMap(char mapName[], Game* game); //loads and display map from a txt file
void printFooter(); //prints footer

//MainTimer.c
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
void moveCar(Game* game, int carId);
void toggleCarDirection(Car* car);
int checkForColision(Game* game, int carId);
void printCar(Game* game, int carId, int prevX);
void setCarLane(Car* car, int lane);
void randomizeMultiplier(Game* game, int carId);
void setUpCar(Game* game, int carId);

#endif
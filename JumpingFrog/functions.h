#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define GAMEBOARDWIDTH 24
#define GAMEBOARDHEIGHT 14
#define ROAD_PAIR 1
#define GRASS_PAIR 2
#define FINISH_PAIR 3

#include <curses.h>

typedef enum {
	PREP,
	START,
	END,
	QUIT
}GameState;

typedef struct {
	int x;
	int y;
} Coordinates;

typedef struct {
	float jumpCooldown;
	Coordinates frogCoords;
}Frog;

typedef struct {
	double remainingTime;
	double initialTime;
	bool isActive;
	void (*onTimerComplete)(void);
}MainTimer;

typedef struct {
	GameState gameState;
	int difficultyLevel;
	MainTimer mainTimer;
	char obstacleCharacter;
	Coordinates finishCords;
	char gameBoard[GAMEBOARDHEIGHT][GAMEBOARDWIDTH];
	Frog frog;
}Game;



//Main.c
void initCurses(); //initialize curses
void initGame(Game* game); //Initialize game
bool canJump(Game game, int userInput); //checks if the frog can make a jump
void jump(int userInput, Game* game); //used for frog movement
void checkForFinish(Game* game); //checks if player reached the finish

//LoadSettings.c
bool loadSettings(Game* game); //Looads and applies settings from txt file

//LoadMap.c
bool loadMap(char mapName[], char gameBoard[GAMEBOARDHEIGHT][GAMEBOARDWIDTH], Game* game); //Loads and display map from a txt file

//MainTimer.c

#endif
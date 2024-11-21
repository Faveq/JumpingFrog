#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <curses.h>	
#include <stdio.h>

#define GAMEBOARDWIDTH 24
#define GAMEBOARDHEIGHT 13
#define MAPSCOUNT 3
#define ROAD 1
#define GRASS 2
#define FINISH 3
#define FOOTER 4



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
	float jumpCooldown; //jump cooldown
	Coordinates coordinates;
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
	int mapNumber;
	MainTimer mainTimer;
	char obstacleCharacter;
	Coordinates finishCoords;
	char gameBoard[GAMEBOARDHEIGHT][GAMEBOARDWIDTH];
	Frog frog;
}Game;

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
bool checkForStart(char ch, int x, int y, char gameBoard[GAMEBOARDHEIGHT][GAMEBOARDWIDTH], Game* game); //checks if loaded fileld is a start field
void display(char ch, int y, int x); //displays loaded char with appropriate color
bool loadMap(char mapName[], char gameBoard[GAMEBOARDHEIGHT][GAMEBOARDWIDTH], Game* game); //loads and display map from a txt file
void printFooter(); //prints footer

//MainTimer.c

#endif
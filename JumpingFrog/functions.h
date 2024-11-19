#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define GAMEBOARDWIDTH 24
#define GAMEBOARDHEIGHT 12
#define ROAD_PAIR 1
#define GRASS_PAIR 2

#include <curses.h>

enum GameState {
    START,
    END
};

struct FrogCoordinates {
    int x;
    int y;
};

struct Frog
{
    float jumpCooldown;
    struct FrogCoordinates frogCoords;
};


struct Game {
    enum GameState state;
    int difficultyLevel;
    int timer;
    char obstacleCharacter;
    char gameBoard[GAMEBOARDHEIGHT][GAMEBOARDWIDTH];
    struct Frog frog;
    int time;
};
struct timeval {
    long tv_sec; /* seconds */
    long tv_usec; /* microseconds */
};

//Main.c
void initCurses(); //initialize curses
int canJump(struct Game game, int userInput); //checks if the frog can make a jump
void jump(int userInput, struct Game* game); //used for frog movement

//LoadSettings.c
void loadSettings(struct Game* game); //Looads and applies settings from txt file

//LoadMap.c
int loadMap(char mapName[], char gameBoard[GAMEBOARDHEIGHT][GAMEBOARDWIDTH], struct FrogCoordinates* frogCoords); //Loads and display map from a txt file

//MainTimer.c
void startTimer(int time);
#endif
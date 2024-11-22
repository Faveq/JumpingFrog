#include "functions.h"

void initTimer(Game* game) {
    game->mainTimer.timeLimit = 0;
    game->mainTimer.timeLeft = 0;
    game->mainTimer.isRunning = 0;
    game->mainTimer.startTime = 0;
}

void setTimeLimit(Game* game, int timeLimit) {
    game->mainTimer.timeLimit = timeLimit;
    game->mainTimer.timeLeft = timeLimit;
}

void startTimer(Game* game) {
    if (!game->mainTimer.isRunning) {
        game->mainTimer.startTime = time(NULL);
        game->mainTimer.isRunning = 1;
    }
}

void stopTimer(Game* game) {
    game->mainTimer.isRunning = 0;
}

void resetTimer(Game* game) {
    game->mainTimer.isRunning = 0;
    game->mainTimer.timeLeft = game->mainTimer.timeLimit;
    game->mainTimer.startTime = 0;
}

void updateTime(Game* game) {
    if (game->mainTimer.isRunning) {
        time_t currentTime = time(NULL);
        double elapsedSeconds = difftime(currentTime, game->mainTimer.startTime);
        game->mainTimer.timeLeft = game->mainTimer.timeLimit - (int)elapsedSeconds;

        if (game->mainTimer.timeLeft <= 0) {
            game->mainTimer.timeLeft = 0;
            game->mainTimer.isRunning = 0;
            game->gameState = LOST;
        }

        printTimer(game);
    }
}

int getTimeLeft(Game* game) {
    return game->mainTimer.timeLeft;
}

int isTimerRunning(Game* game) {
    return game->mainTimer.isRunning;
}

void printTimer(Game* game) {
    mvprintw(GAMEBOARDHEIGHT + 1, 1, "Time left: %02d", game->mainTimer.timeLeft);
    refresh();
}
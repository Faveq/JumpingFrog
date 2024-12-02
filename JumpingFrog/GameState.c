#include "functions.h"

const char* maps[MAPSCOUNT] = {
	"maps/map1.txt",
	"maps/map2.txt",
	"maps/map3.txt"
};
void handleStartState(Game* game) {
	double currentTime = getCurrentTimeInMs();
	double elapsedTime = currentTime - game->lastMove;

	if (!game->mainTimer.isRunning) {
		startTimer(game);
	}
	updateTime(game);

	if (game->mainTimer.timeLeft % 10 == 5) {
		for (int i = 0; i < ROADSCOUNT; i++) {
			randomizeMultiplier(game, i);
		}
	}
	if (elapsedTime > 70)
	{
		for (int i = 0; i < ROADSCOUNT; i++) {
			moveCar(game, i);
		}
		game->lastMove = getCurrentTimeInMs();
	}

	timeout(100);
	int userInput = getch();

	if (userInput != ERR) {
			if (userInput == 'q' || userInput == 'Q') {
				game->gameState = QUIT;
				return;
			}
			else if (userInput == 'r' || userInput == 'R') {
				game->gameState = PREP;
				return;
			}
			else {
				jump(userInput, game);
			}
	}

	refresh();
}

void handleWonState(Game* game) {
	stopTimer(game);
	clear();
	mvprintw(7, 15, "YOU HAVE REACHED THE FINISH CONGRATS!!!");
	mvprintw(8, 25, "press r for next map");
	refresh();

	int userInput = getch();
	if (userInput == 'r' || userInput == 'R') {
		game->gameState = PREP;
	}
	else if (userInput == 'q' || userInput == 'Q') {
		game->gameState = QUIT;
	}
}

void handleLostState(Game* game) {
	stopTimer(game);
	clear();
	mvprintw(7, 10, "YOU LOST");
	mvprintw(8, 4, "press r for restart");
	refresh();

	int userInput = getch();
	if (userInput == 'r' || userInput == 'R') {
		game->gameState = PREP;
	}
	else if (userInput == 'q' || userInput == 'Q') {
		game->gameState = QUIT;
	}
}

bool prepareGameResources(Game* game) {
	if (!loadAssets(&game->assets)) {
		mvprintw(5, 5, "Failed to load assets");
		return 0;
	}

	if (!loadMap(maps[game->mapNumber], game)) {
		mvprintw(5, 5, "Failed to load map");
		return 0;
	}

	if (!loadSettings(game)) {
		mvprintw(5, 5, "Failed to load settings");
		return 0;
	}

	game->gameState = START;
	for (int i = 0; i < ROADSCOUNT; i++) {
		setUpCar(game, i);
	}

	refresh();
	printFrog(game->frog.coordinates.y * ASSETHEIGHT,
		game->frog.coordinates.x * ASSETWIDTH,
		&game->assets);
	renderFrogMovement(-1, -1, game->frog.coordinates.y,
		game->frog.coordinates.x, *game);
	refresh();

	return 1;
}

void handlePrepState(Game* game) {
	resetTimer(game);
	clear();
	refresh();

	if (!prepareGameResources(game)) {
		mvprintw(0, 0, "ERROR");
		refresh();
		game->gameState = QUIT;
		(void)getchar();
	}
}

void handleGameStates(Game* game) {
	switch (game->gameState) {
	case START:
		handleStartState(game);
		break;
	case WON:
		handleWonState(game);
		break;
	case LOST:
		handleLostState(game);
		break;
	case PREP:
		handlePrepState(game);
		break;
	default:
		break;
	}
}

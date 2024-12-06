#include "functions.h"

const char* maps[MAPSCOUNT] = {
	"maps/map1.txt",
	"maps/map2.txt",
	"maps/map3.txt"
};

static void printStatus(Game* game, char* message) {
	int startY = 10;
	for (int i = 0; i <= COLS; i++)
	{
		mvprintw(startY, i, "=");
		mvprintw(startY + 3, i, "=");
	}
	mvprintw(startY + 1, 20, message);
	mvprintw(startY + 2, 33, "Time left: %d", getTimeLeft(game));

	static int blinkCounter = 0;
	blinkCounter++;

	if (blinkCounter % 7 < 3) {
		mvprintw(startY + 4, 38, "R");
	}
}
void handleStartState(Game* game) {
	double currentTime = getCurrentTimeInMs();
	double elapsedTime = currentTime - game->lastMove;

	if (!game->mainTimer.isRunning) {
		startTimer(game);
	}
	updateTime(game);

	
	if (game->mainTimer.timeLeft % ((MAPSCOUNT - game->mapNumber) * 3) == 0) {
		for (int i = 0; i < ROADSCOUNT; i++) {
			randomizeMultiplier(game, i);
		}
	}
	//prevents cars from speeding while button is cliked
	if (elapsedTime > 50)
	{
		for (int i = 0; i < ROADSCOUNT; i++) {
			moveCar(game, i);
		}
		game->lastMove = getCurrentTimeInMs();
	}

	timeout(1);
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
	printStatus(game,"YOU HAVE REACHED THE FINISH CONGRATS!!!");
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
	switch (game->lostBy)
	{
	case CAR:
		printStatus(game, "IT'S UNFORTUNATE BUT YOU GOT RUN OVER");
		break;
	case TIME:
		printStatus(game, "YOU SEEMS TOO SLOW, HURRY UP A LITTLE!");
		break;
	default:
		break;
	}
	refresh();

	int userInput = getch();
	if (userInput == 'r' || userInput == 'R') {
		game->gameState = PREP;
	}
	else if (userInput == 'q' || userInput == 'Q') {
		game->gameState = QUIT;
	}
}

int prepareGameResources(Game* game) {
	if (!loadAssets(&game->assets)) {
		mvprintw(5, 5, "Failed to load assets");
		return 0;
	}

	if (!loadSettings(game)) {
		mvprintw(5, 5, "Failed to load settings");
		return 0;
	}

	if (!loadMap(maps[game->mapNumber], game)) {
		mvprintw(5, 5, "Failed to load map");
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

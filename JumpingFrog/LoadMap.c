#include "functions.h"

bool checkForStart(char ch, int x, int y, Game* game) {
	if (ch == 'S') {
		game->frog.coordinates.y = y;
		game->frog.coordinates.x = x;
		game->gameBoard[y][x] = ' ';
		return 1;
	}
	return 0;
}

void display(char ch, int y, int x, Game* game, RoadsTracker *roadsTracker) {
	if (ch == 'F') {
		activateColor(FINISH);
		printGrass(y * ASSETHEIGHT, x * ASSETWIDTH, &game->assets);
		deactivateColor(FINISH);
	}
	else if (ch == '-') {
		activateColor(ROAD);
		printRoad(y * ASSETHEIGHT, x * ASSETWIDTH, &game->assets);
		deactivateColor(ROAD);
		if (y != roadsTracker->prevLine)
		{
			roadsTracker->prevLine = y;
			game->cars[roadsTracker->carsNumber].coordinates.y = roadsTracker->prevLine;
			roadsTracker->carsNumber++;
		}
	}
	else if(ch == ' ')
	{
		activateColor(GRASS);
		printGrass(y * ASSETHEIGHT, x * ASSETWIDTH, &game->assets);
		deactivateColor(GRASS);
	}
	else {
		activateColor(GRASS);
		printBlocade(y * ASSETHEIGHT, x * ASSETWIDTH, &game->assets);
		deactivateColor(GRASS);
	}
}

bool loadMap(char mapName[], Game* game) {
	FILE* file = fopen(mapName, "r");
	RoadsTracker roadsTracker = { .prevLine = 0, .carsNumber = 0 };
	if (!file) return 0;

	int x = 0, y = 0, ch;
	while ((ch = fgetc(file)) != EOF) {
		if (x == GAMEBOARDWIDTH) {
			y++;
			x = 0;
		}
		else {
			if (!checkForStart(ch, x, y, game)) {
				if (ch == 'F') {
					game->finishCoords.y = y;
					game->finishCoords.x = x;
				}
				game->gameBoard[y][x] = ch;
				display(ch, y, x, game, &roadsTracker);
			}
			x++;
		}
	}

	fclose(file);
	printFooter();
	return 1;
}

void printFooter() {
	activateColor(FOOTER);
	for (int i = 0; i <= COLS; i++)
	{
		for (int j = LINES - 2; j < LINES; j++)
		{
			mvprintw(j, i, " ");
		}
	}
	mvprintw(LINES - 2, 0, "Bartosz Œwierczyñski");
	mvprintw(LINES - 1, 0, "203825");
	deactivateColor(FOOTER);
	refresh();
}
#include "functions.h"

bool checkForStart(char ch, int x, int y, char gameBoard[GAMEBOARDHEIGHT][GAMEBOARDWIDTH], Game* game) {
	if (ch == 'S') {
		game->frog.coordinates.y = y;
		game->frog.coordinates.x = x;
		gameBoard[y][x] = ' ';
		return 1;
	}
	return 0;
}

void display(char ch, int y, int x) {
	if (ch == 'F') {
		activateColor(FINISH);
		mvaddch(y, x, ' ');
		deactivateColor(FINISH);
	}
	else if (ch == '-') {
		activateColor(ROAD);
		mvaddch(y, x, ch);
		deactivateColor(ROAD);
	}
	else {
		activateColor(GRASS);
		mvaddch(y, x, ch);
		deactivateColor(GRASS);
	}
}

bool loadMap(char mapName[], char gameBoard[GAMEBOARDHEIGHT][GAMEBOARDWIDTH], Game* game) {
	FILE* file = fopen(mapName, "r");
	if (!file) return 0;

	int x = 0, y = 0, ch;
	while ((ch = fgetc(file)) != EOF) {
		if (x == GAMEBOARDWIDTH) {
			y++;
			x = 0;
		}
		else {
			if (!checkForStart(ch, x, y, gameBoard, game)) {
				if (ch == 'F') {
					game->finishCoords.y = y;
					game->finishCoords.x = x;
				}
				display(ch, y, x);
				gameBoard[y][x] = ch;
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
	//mvprintw(LINES - 2, 0, "Bartosz Œwierczyñski");
	//mvprintw(LINES - 1, 0, "203825");
	deactivateColor(FOOTER);
	refresh();
}
#include "functions.h"
#include <curses.h>
#include <stdio.h>

int loadMap(char mapName[], char gameBoard[GAMEBOARDHEIGHT][GAMEBOARDWIDTH], struct FrogCoordinates* frogCoords) {

	FILE* file = fopen(mapName, "r");
	if (!file) {
		perror("Failed to open the file");
		return 0;
	}

	int x = 0, y = 0;
	int ch;

	while ((ch = fgetc(file)) != EOF) {
		if (ch == '\n') {
			y++;
			x = 0;
		}
		else if (ch == 'S') {
			frogCoords->x = x;
			frogCoords->y = y;
		}
		else {
			if (ch == '-') {
				attron(COLOR_PAIR(ROAD_PAIR));
				mvaddch(y, x, ch);
				attroff(COLOR_PAIR(ROAD_PAIR));
			}
			else {
				attron(COLOR_PAIR(GRASS_PAIR));
				mvaddch(y, x, ch);
				attroff(COLOR_PAIR(GRASS_PAIR));

			}
			gameBoard[y][x] = ch;
			x++;
		}
	}

	fclose(file);
	return 1;
}
#include "functions.h"
#include <curses.h>
#include <stdio.h>

bool loadMap(char mapName[], char gameBoard[GAMEBOARDHEIGHT][GAMEBOARDWIDTH], Game* game) {

	FILE* file = fopen(mapName, "r");
	if (!file) {
		perror("Failed to open the file");
		return 0;
	}

	int x = 0, y = 0;
	int ch;

	while ((ch = fgetc(file)) != EOF) {
		if (x == GAMEBOARDWIDTH) {
			y++;
			x = 0;
		}
		else {
			if (ch == 'S') {
				game->frog.frogCoords.y = y;
				game->frog.frogCoords.x = x;
				gameBoard[y][x] = ' ';
			}
			else {
				if (ch == 'F')
				{
					game->finishCords.y = y;
					game->finishCords.x = x;
					attron(COLOR_PAIR(FINISH_PAIR));
					mvaddch(y, x, ' ');
					attroff(COLOR_PAIR(FINISH_PAIR));
				}
				else if (ch == '-') {
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
			}
			
			x++;
		}
	}


	fclose(file);
	return 1;
}
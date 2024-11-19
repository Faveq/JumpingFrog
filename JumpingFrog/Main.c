#include "functions.h"
#include <curses.h>

void initCurses() {
	initscr();
	noecho();
	start_color();
	keypad(stdscr, TRUE);
	curs_set(0);
	init_pair(ROAD_PAIR, COLOR_WHITE, COLOR_BLACK);
	init_pair(GRASS_PAIR, COLOR_BLACK, COLOR_GREEN);
	init_pair(FINISH_PAIR, COLOR_BLACK, COLOR_RED);
}

void initGame(Game *game) {
	game->gameState = PREP;
	game->difficultyLevel = 0;
	game->mainTimer = (MainTimer){ .remainingTime = 0, .initialTime = 0, .isActive = 0 };
	game->obstacleCharacter = ' ';
	game->finishCords = (Coordinates){ .y = 0, .x = 0 };
	memset(game->gameBoard, ' ', sizeof(game->gameBoard));
	game->frog.jumpCooldown = 0;
	game->frog.frogCoords = (Coordinates){ .y = 0, .x = 0 };
}

bool canJump(Game game, int userInput) {
	switch (userInput) {
	case KEY_UP:
		if (game.frog.frogCoords.y > 0 &&
			game.gameBoard[game.frog.frogCoords.y - 1][game.frog.frogCoords.x] != game.obstacleCharacter)
			return 1;
		break;
	case KEY_DOWN:
		if (game.frog.frogCoords.y < GAMEBOARDHEIGHT - 1 &&
			game.gameBoard[game.frog.frogCoords.y + 1][game.frog.frogCoords.x] != game.obstacleCharacter)
			return 1;
		break;
	case KEY_LEFT:
		if (game.frog.frogCoords.x > 0 &&
			game.gameBoard[game.frog.frogCoords.y][game.frog.frogCoords.x - 1] != game.obstacleCharacter)
			return 1;
		break;
	case KEY_RIGHT:
		if (game.frog.frogCoords.x < GAMEBOARDWIDTH - 1 &&
			game.gameBoard[game.frog.frogCoords.y][game.frog.frogCoords.x + 1] != game.obstacleCharacter)
			return 1;
		break;
	}
	return 0;
}

void changeFrogPositon(int prevY, int prevX, int y, int x, Game game)
{
	if (prevY != -1 && prevX != -1) {
		char znak = game.gameBoard[prevY][prevX];
		if (game.gameBoard[prevY][prevX] == '-') {
			attron(COLOR_PAIR(ROAD_PAIR));
			mvprintw(prevY, prevX, "%c", znak);
			attroff(COLOR_PAIR(ROAD_PAIR));
		}
		else if (znak == ' ')
		{
			attron(COLOR_PAIR(GRASS_PAIR));
			mvprintw(prevY, prevX, "%c", znak);
			attroff(COLOR_PAIR(GRASS_PAIR));
		}
	}

	if (game.gameBoard[game.frog.frogCoords.y][game.frog.frogCoords.x] == '-')
	{
		attron(COLOR_PAIR(ROAD_PAIR));
		mvprintw(game.frog.frogCoords.y, game.frog.frogCoords.x, "X");
		attroff(COLOR_PAIR(ROAD_PAIR));
	}
	else
	{
		attron(COLOR_PAIR(GRASS_PAIR));
		mvprintw(game.frog.frogCoords.y, game.frog.frogCoords.x, "X");
		attroff(COLOR_PAIR(GRASS_PAIR));
	}
}

void jump(int userInput, Game* game) {
	int prevY = game->frog.frogCoords.y;
	int prevX = game->frog.frogCoords.x;

	mvprintw(game->frog.frogCoords.y, game->frog.frogCoords.x, " ");

	switch (userInput) {
	case KEY_UP:
		if (canJump(*game, userInput)) game->frog.frogCoords.y--;
		break;
	case KEY_DOWN:
		if (canJump(*game, userInput)) game->frog.frogCoords.y++;
		break;
	case KEY_LEFT:
		if (canJump(*game, userInput)) game->frog.frogCoords.x--;
		break;
	case KEY_RIGHT:
		if (canJump(*game, userInput)) game->frog.frogCoords.x++;
		break;
	}

	changeFrogPositon(prevY, prevX, game->frog.frogCoords.y, game->frog.frogCoords.x, *game);
	
	checkForFinish(game);
	
	refresh();
}

void checkForFinish(Game *game) {
	if (game->frog.frogCoords.x == game->finishCords.x && game->frog.frogCoords.y == game->finishCords.y)
		game->gameState = END;
}

int main() {
	int userInput;
	Game game;
	initGame(&game);
	initCurses();

	while (game.gameState != QUIT)
	{
		if (game.gameState == PREP)
		{
			clear();
			refresh();
			if (loadMap("map1.txt", game.gameBoard, &game) && loadSettings(&game))
			{

				game.gameState = START;
				//for (int i = 0; i < GAMEBOARDHEIGHT; i++) {
				//    for (int j = 0; j < GAMEBOARDWIDTH; j++) {
				//        mvprintw(i, j, "%c", game.gameBoard[i][j]);
				//    }
				//}
				refresh();

				mvprintw(game.frog.frogCoords.y, game.frog.frogCoords.x, "X");
				changeFrogPositon(-1, -1, game.frog.frogCoords.y, game.frog.frogCoords.x, game);
				refresh();

				while (game.gameState == START) {
					userInput = getch();

					if (userInput == 'q' || userInput == 'Q') {
						break;
					}

					jump(userInput, &game);
				}
			}
			else {

				mvprintw(5, 5, "FAILED TO LOAD THE MAP");
			}
		}
		if (game.gameState == END)
		{
			clear();
			mvprintw(7, 15, "YOU HAVE REACHED THE FINISH CONGRATS!!!");
			mvprintw(8, 25, "press r for restart");
			refresh();
			userInput = getch();

			if (userInput == 'r' || userInput == 'R')
			{
				game.gameState = PREP;
			}

		}
	}
	endwin();
	return 0;
}

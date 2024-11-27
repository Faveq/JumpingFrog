#include "functions.h"

void initCurses() {
	initscr();
	noecho();
	start_color();
	keypad(stdscr, TRUE);
	curs_set(0);
	init_pair(ROAD, COLOR_WHITE, COLOR_BLACK);
	init_pair(GRASS, COLOR_BLACK, COLOR_GREEN);
	init_pair(FINISH, COLOR_BLACK, COLOR_RED);
	init_pair(FOOTER, COLOR_BLACK, COLOR_YELLOW);
}

void initGame(Game *g) {
	g->gameState = PREP;
	g->difficultyLevel = 0;
	g->mapNumber = 0;
	g->obstacleCharacter = ' ';
	g->finishCoords = (Coordinates){ .y = 0, .x = 0 };
	memset(g->gameBoard, ' ', sizeof(g->gameBoard));
	g->frog.jumpCooldown = 0;
	g->frog.coordinates = (Coordinates){ .y = 0, .x = 0 };
	for (int i = 0; i < ROADSCOUNT; i++) {
		g->cars[i].coordinates.x = 0;
		g->cars[i].direction = 1;
	}
	initTimer(g);
}

void resetGame(Game *g) {
	for (int i = 0; i < ROADSCOUNT; i++) {
		g->cars[i].coordinates.x = 0;
		g->cars[i].direction = 1;
	}
	memset(g->gameBoard, ' ', sizeof(g->gameBoard));
}

void activateColor(int colorPair) {
	attron(COLOR_PAIR(colorPair));
}

void deactivateColor(int colorPair) {
	attroff(COLOR_PAIR(colorPair));
}

bool canJump(Game game, int userInput) {
	int ny = game.frog.coordinates.y,
		nx = game.frog.coordinates.x;

	switch (userInput) {
	case KEY_UP: ny--; break;
	case KEY_DOWN: ny++; break;
	case KEY_LEFT: nx--; break;
	case KEY_RIGHT: nx++; break;
	}

	return (ny >= 0 && ny < GAMEBOARDHEIGHT &&
		nx >= 0 && nx < GAMEBOARDWIDTH &&
		game.gameBoard[ny][nx] != game.obstacleCharacter && 
		game.gameBoard[ny][nx] != 'C');
}

void changeFrogPositon(int prevY, int prevX, int y, int x, Game game)
{
	if (prevY != -1 && prevX != -1) {
		char znak = game.gameBoard[prevY][prevX];
		if (game.gameBoard[prevY][prevX] == '-') {
			activateColor(ROAD);
			mvprintw(prevY, prevX, "%c", znak);
			deactivateColor(ROAD);
		}
		else if (znak == ' ')
		{
			activateColor(GRASS);
			mvprintw(prevY, prevX, "%c", znak);
			deactivateColor(GRASS);
		}
	}

	if (game.gameBoard[game.frog.coordinates.y][game.frog.coordinates.x] == '-')
	{
		activateColor(ROAD);
		mvprintw(game.frog.coordinates.y, game.frog.coordinates.x, "X");
		deactivateColor(ROAD);
	}
	else
	{
		activateColor(GRASS);

		mvprintw(game.frog.coordinates.y, game.frog.coordinates.x, "X");
		deactivateColor(GRASS);
	}
}

void jump(int userInput, Game* game) {
	int prevY = game->frog.coordinates.y;
	int prevX = game->frog.coordinates.x;

	mvprintw(game->frog.coordinates.y, game->frog.coordinates.x, " ");

	switch (userInput) {
	case KEY_UP:
		if (canJump(*game, userInput)) game->frog.coordinates.y--;
		break;
	case KEY_DOWN:
		if (canJump(*game, userInput)) game->frog.coordinates.y++;
		break;
	case KEY_LEFT:
		if (canJump(*game, userInput)) game->frog.coordinates.x--;
		break;
	case KEY_RIGHT:
		if (canJump(*game, userInput)) game->frog.coordinates.x++;
		break;
	}

	changeFrogPositon(prevY, prevX, game->frog.coordinates.y, game->frog.coordinates.x, *game);
	
	checkForFinish(game);
	
	refresh();
}

void checkForFinish(Game *game) {
	if (game->frog.coordinates.x == game->finishCoords.x && game->frog.coordinates.y == game->finishCoords.y)
	{
		if (game->mapNumber < MAPSCOUNT - 1)
		{
			game->mapNumber++;
		}
		else {
			game->mapNumber = 0;
		}
		game->gameState = WON;
	}
}
const char* maps[MAPSCOUNT] = {
	"map1.txt",
	"map2.txt",
	"map3.txt"
};
int main() {
	int userInput;
	Game game;
	initGame(&game);
	initCurses();

	while (game.gameState != QUIT)
	{
		if (game.gameState == PREP)
		{
			resetGame(&game);
			resetTimer(&game);
			clear();
			refresh();
			if (loadMap(maps[game.mapNumber], &game)&& loadSettings(&game))
			{

				game.gameState = START;
				for (int i = 0; i < ROADSCOUNT; i++)
				{
					setUpCar(&game, i);
				}
				for (int i = 0; i < GAMEBOARDHEIGHT; i++) {
				    for (int j = 0; j < GAMEBOARDWIDTH; j++) {
				        mvprintw(i + 16, j , "%c", game.gameBoard[i][j]);
				    }
				}
				refresh();

				mvprintw(game.frog.coordinates.y, game.frog.coordinates.x, "X");
				changeFrogPositon(-1, -1, game.frog.coordinates.y, game.frog.coordinates.x, game);
				refresh();

				while (game.gameState == START) {
					if (!game.mainTimer.isRunning) {
						startTimer(&game);
					}
					updateTime(&game);

					timeout(100);
					userInput = getch();

					if (userInput != ERR) {
						if (userInput == 'q' || userInput == 'Q') {
							game.gameState = QUIT;
							break;
						}else if (userInput == 'r' || userInput == 'R') {
							game.gameState = PREP;
							break;
						}
						jump(userInput, &game);
					}

					refresh();
				}
			}
			else {

				mvprintw(5, 5, "FAILED TO LOAD THE MAP");
			}
		}
		if (game.gameState == WON)
		{
			stopTimer(&game);
			clear();
			mvprintw(7, 15, "YOU HAVE REACHED THE FINISH CONGRATS!!!");
			mvprintw(8, 25, "press r for restart");
			refresh();
			userInput = getch();

			if (userInput == 'r' || userInput == 'R')
			{
				game.gameState = PREP;
			}
			else if (userInput == 'q' || userInput == 'Q') {
					game.gameState = QUIT;
					break;
				
			}

		}
		else if (game.gameState == LOST)
		{
			stopTimer(&game);
			clear();
			mvprintw(7, 10, "YOU LOST");
			mvprintw(8, 4, "press r for restart");
			refresh();
			userInput = getch();

			if (userInput == 'r' || userInput == 'R')
			{
				game.gameState = PREP;
			}else if (userInput == 'q' || userInput == 'Q') {
				game.gameState = QUIT;
				break;

			}
		}
	}
	endwin();
	return 0;
}

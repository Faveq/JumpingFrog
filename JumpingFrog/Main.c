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

void activateColor(int colorPair) {
	attron(COLOR_PAIR(colorPair));
}

void deactivateColor(int colorPair) {
	attroff(COLOR_PAIR(colorPair));
}

void initGame(Game* g) {
	g->gameState = PREP;
	g->difficultyLevel = 0;
	g->mapNumber = 0;
	g->obstacleCharacter = ' ';
	g->finishCoords = (Coordinates){ .y = 0, .x = 0 };
	memset(g->gameBoard, ' ', sizeof(g->gameBoard));
	g->frog.jumpCooldown = 0;
	g->frog.coordinates = (Coordinates){ .y = 0, .x = 0 };
	g->frog.lastJumpTime = getCurrentTimeInMs();
	for (int i = 0; i < ROADSCOUNT; i++) {
		g->cars[i].coordinates.x = 0;
		g->cars[i].direction = 1;
		g->cars[i].lastMoved = getCurrentTimeInMs();
	}
	memset(g->assets.blocadePrint, ' ', sizeof(g->assets.blocadePrint));
	memset(g->assets.frogPrint, ' ', sizeof(g->assets.frogPrint));
	memset(g->assets.carPrint, ' ', sizeof(g->assets.carPrint));
	memset(g->assets.grassPrint, ' ', sizeof(g->assets.grassPrint));
	memset(g->assets.roadPrint, ' ', sizeof(g->assets.roadPrint));
	g->lastMove= getCurrentTimeInMs();
	initTimer(g);
}

int checkForJumpCooldown(Frog* frog) {
	double currentTime = getCurrentTimeInMs();
	double elapsedTime = currentTime - frog->lastJumpTime;

	if (elapsedTime >= frog->jumpCooldown) {
		frog->lastJumpTime = currentTime;
		return 1;
	}
	return 0;
}

int canJump(Game game, int userInput) {
	int newY = game.frog.coordinates.y,
		newX = game.frog.coordinates.x;

	switch (userInput) {
	case KEY_UP: newY--; break;
	case KEY_DOWN: newY++; break;
	case KEY_LEFT: newX--; break;
	case KEY_RIGHT: newX++; break;
	}

	return (newY >= 0 && newY < GAMEBOARDHEIGHT &&
		newX >= 0 && newX < GAMEBOARDWIDTH &&
		game.gameBoard[newY][newX] != game.obstacleCharacter &&
		game.gameBoard[newY][newX] != 'C');
}

void renderFrogMovement(int prevY, int prevX, int y, int x, Game game)
{
	if (prevY != -1 && prevX != -1) {
		char znak = game.gameBoard[prevY][prevX];
		if (game.gameBoard[prevY][prevX] == '-') {
			activateColor(ROAD);
			printRoad(prevY * ASSETHEIGHT, prevX * ASSETWIDTH, &game.assets);
			deactivateColor(ROAD);
		}
		else if (znak == ' ')
		{
			activateColor(GRASS);
			printGrass(prevY * ASSETHEIGHT, prevX * ASSETWIDTH, &game.assets);
			deactivateColor(GRASS);
		}
	}

	if (game.gameBoard[game.frog.coordinates.y][game.frog.coordinates.x] == '-')
	{
		activateColor(ROAD);
		printFrog(game.frog.coordinates.y * ASSETHEIGHT, game.frog.coordinates.x * ASSETWIDTH, &game.assets);
		deactivateColor(ROAD);
	}
	else
	{
		activateColor(GRASS);
		printFrog(game.frog.coordinates.y * ASSETHEIGHT, game.frog.coordinates.x * ASSETWIDTH, &game.assets);
		deactivateColor(GRASS);
	}
}

void jump(int userInput, Game* game) {
	if (checkForJumpCooldown(&game->frog))
	{
		int prevY = game->frog.coordinates.y;
		int prevX = game->frog.coordinates.x;

		printGrass(game->frog.coordinates.y * ASSETHEIGHT, game->frog.coordinates.x * ASSETWIDTH, &game->assets);

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
		renderFrogMovement(prevY, prevX, game->frog.coordinates.y, game->frog.coordinates.x, *game);

		checkForFinish(game);

		refresh();
	}
}

void checkForFinish(Game* game) {
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

int main() {
	Game game;

	initGame(&game);
	initCurses();

	while (game.gameState != QUIT) {
		handleGameStates(&game);
	}

	endwin();
	return 0;
}

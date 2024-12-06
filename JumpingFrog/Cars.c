#include "functions.h"

int choose_random(int firstCoord, int secondCoord) {
	int choice = rand() % 2;
	return (choice == 0) ? firstCoord : secondCoord;
}

void radomizeStartSite(Game* game, int carId) {
	game->cars[carId].coordinates.x = choose_random(0, GAMEBOARDWIDTH - 1);
	checkForColision(game, carId);
}

void moveCar(Game* game, int carId) {
	int prevX = game->cars[carId].coordinates.x;
	double defaultTime = 350;
	double currentTime = getCurrentTimeInMs();

	checkForColision(game, carId);


	game->gameBoard[game->cars[carId].coordinates.y][prevX] = '-';

	double elapsedTime = currentTime - game->cars[carId].lastMoved;

	double timePerMove = defaultTime * (MAPSCOUNT - game->mapNumber)/MAPSCOUNT / game->cars[carId].speedMultiplier;

	if (elapsedTime >= timePerMove)
	{
		if (game->cars[carId].coordinates.x < GAMEBOARDWIDTH - 1 && game->cars[carId].coordinates.x >= 0)
		{
			game->cars[carId].coordinates.x += game->cars[carId].direction;
		}
		else {

			radomizeStartSite(game, carId);
			if (game->cars[carId].coordinates.x == GAMEBOARDWIDTH - 1 && game->cars[carId].direction == 1 || game->cars[carId].coordinates.x == 0 && game->cars[carId].direction == -1)
			{
				toggleCarDirection(&game->cars[carId]);
			}
			game->cars[carId].coordinates.x += game->cars[carId].direction;
		}

		if (game->cars[carId].coordinates.y >= 0 &&
			game->cars[carId].coordinates.y < GAMEBOARDHEIGHT && game->cars[carId].coordinates.x >= 0 &&
			game->cars[carId].coordinates.x < GAMEBOARDWIDTH) {
			game->gameBoard[game->cars[carId].coordinates.y][game->cars[carId].coordinates.x] = 'C';
		}


		printCar(game->cars[carId].coordinates.y * ASSETHEIGHT, game->cars[carId].coordinates.x * ASSETWIDTH, game, prevX, carId);
		checkForColision(game, carId);
		game->cars[carId].lastMoved = getCurrentTimeInMs();
	}
}

void toggleCarDirection(Car* car) {
	car->direction *= (-1);
}

int checkForColision(Game* game, int carId) {
	if (game->cars[carId].coordinates.x == game->frog.coordinates.x && game->cars[carId].coordinates.y == game->frog.coordinates.y)
	{
		game->lostBy = CAR;
		game->gameState = LOST;
	}
}

void randomizeMultiplier(Game* game, int carId) {
	game->cars[carId].speedMultiplier = rand() % game->difficultyLevel * (game->mapNumber + 1) + 3;
	refresh();
}

void setUpCar(Game* game, int carId) {
	radomizeStartSite(game, carId);
	randomizeMultiplier(game, carId);

}
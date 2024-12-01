#include "functions.h"

int choose_random(int firstCoord, int secondCoord) {
	int choice = rand() % 2;
	return (choice == 0) ? firstCoord : secondCoord;
}

void radomizeStartSite(Game* game, int carId) {
	game->cars[carId].coordinates.x = choose_random(0, GAMEBOARDWIDTH - 1);
}

void moveCar(Game* game, int carId) {
	int prevX = game->cars[carId].coordinates.x;
	checkForColision(game, carId);
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
	game->gameBoard[game->cars[carId].coordinates.y][prevX] = '-';
	game->gameBoard[game->cars[carId].coordinates.y][game->cars[carId].coordinates.x] = 'C';
	

	printCar(game->cars[carId].coordinates.y * ASSETHEIGHT, game->cars[carId].coordinates.x * ASSETWIDTH, game, prevX, carId);
	checkForColision(game, carId);
}

void toggleCarDirection(Car* car) {
	car->direction *= (-1);
}

int checkForColision(Game* game, int carId) {
	if (game->cars[carId].coordinates.x == game->frog.coordinates.x && game->cars[carId].coordinates.y == game->frog.coordinates.y)
	{
		game->gameState = LOST;
	}
}

void randomizeMultiplier(Game *game, int carId) {
	game->cars[carId].speedMultiplier = rand() % game->difficultyLevel + 1;
}

void setUpCar(Game* game, int carId) {
	radomizeStartSite(game, carId);
	randomizeMultiplier(game, carId);

}
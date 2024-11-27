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
	

	printCar(game, carId, prevX);
	checkForColision(game, carId);

	//mvprintw(15, 0, "Car x: %d y: %d", game->cars[carId].coordinates.x, game->cars[carId].coordinates.y);
	//mvprintw(16, 0, "GAMEBOARDWIDTH %d", GAMEBOARDWIDTH);
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

void printCar(Game* game, int carId, int prevX) {
	Car* car = &game->cars[carId];


	if ((car->coordinates.x - car->direction) < GAMEBOARDWIDTH)
	{
		mvprintw(car->coordinates.y, prevX, "-");
	}
	mvprintw(car->coordinates.y, car->coordinates.x, "C");
}

void setCarLane(Car *car, int lane) {
		car->coordinates.y = lane;
}

void randomizeMultiplier(Game *game, int carId) {
	game->cars[carId].speedMultiplier = (rand() % (1 - game->difficultyLevel)) + 1;
}
void setUpCar(Game* game, int carId) {
	radomizeStartSite(game, carId);
	randomizeMultiplier(game, carId);

}
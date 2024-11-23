#include "functions.h"

void moveCar(Game* game) {
	checkForColision(game);
	if (game->car.coordinates.x < GAMEBOARDWIDTH - 1 && game->car.coordinates.x >= 0)
	{
		game->car.coordinates.x += game->car.direction;
	}
	else {
		toggleCarDirection(&game->car);
		game->car.coordinates.x += game->car.direction;
	}
	game->gameBoard[game->car.coordinates.y][game->car.coordinates.x - game->car.direction] = ' ';
	game->gameBoard[game->car.coordinates.y][game->car.coordinates.x] = 'C';

	printCar(game);
	checkForColision(game);

	mvprintw(15, 0, "Car x: %d y: %d", game->car.coordinates.x, game->car.coordinates.y);
	mvprintw(16, 0, "GAMEBOARDWIDTH %d", GAMEBOARDWIDTH);
}

void toggleCarDirection(Car* car) {
	car->direction *= (-1);
}

int checkForColision(Game* game) {
	if (game->car.coordinates.x == game->frog.coordinates.x && game->car.coordinates.y == game->frog.coordinates.y)
	{
		game->gameState = LOST;
	}
}

void printCar(Game* game) {
	Car* car = &game->car;
	if (car->coordinates.x < GAMEBOARDWIDTH && car->coordinates.x > 0)
		game->gameBoard[car->coordinates.y][car->coordinates.x - car->direction] = '-';

	if ((car->coordinates.x - car->direction) < GAMEBOARDWIDTH)
	{
		mvprintw(car->coordinates.y, car->coordinates.x - car->direction, "-");
	}
	mvprintw(car->coordinates.y, car->coordinates.x, "X");
}
#include "functions.h"

void printBlocade(int y, int x, Assets* asset)
{
	print(y, x, asset->blocadePrint);
}

void printCar(int y, int x, Game* game, int prevX, int carId)
{
	Car* car = &game->cars[carId];

	if ((car->coordinates.x - car->direction) < GAMEBOARDWIDTH)
	{
		printRoad(car->coordinates.y * ASSETHEIGHT, prevX * ASSETWIDTH, &game->assets);
	}
	print(car->coordinates.y * ASSETHEIGHT, car->coordinates.x*ASSETWIDTH, game->assets.carPrint);
}

void printFrog(int y, int x, Assets* asset)
{
	print(y, x, asset->frogPrint);
}

void printRoad(int y, int x, Assets* printables)
{
	print(y, x, printables->roadPrint);
}

void printGrass(int y, int x, Assets* printables)
{
	print(y, x, printables->grassPrint);
}

void print(int y, int x, char asset[ASSETHEIGHT][ASSETWIDTH]) {
	int countX = 0, countY = 0;
	for (int i = y; i < y + ASSETHEIGHT; i++, countY++)
	{
		for (int j = x; j < x + ASSETWIDTH; j++, countX++)
		{
			mvprintw(i, j, "%c", asset[countY][countX]);
		}
		countX = 0;
	}
	refresh();
}

int loadAssets(Assets *assets) {
	FILE *frogFile = fopen("assets/frog.txt", "r");
	if (!frogFile)
		return 0;
	mapAsset(frogFile, assets->frogPrint);
	fclose(frogFile);

	FILE* carFile = fopen("assets/car.txt", "r");
	if (!carFile)
		return 0;
	mapAsset(carFile, assets->carPrint);
	fclose(carFile);

	FILE* blocadeFile = fopen("assets/blocade.txt", "r");
	if (!blocadeFile)
		return 0;
	mapAsset(blocadeFile, assets->blocadePrint);
	fclose(blocadeFile);

	FILE* roadFile = fopen("assets/road.txt", "r");
	if (!roadFile)
		return 0;
	mapAsset(roadFile, assets->roadPrint);
	fclose(roadFile);
	return 1;

}

void mapAsset(FILE* file, char asset[ASSETHEIGHT][ASSETWIDTH]) {
	int x = 0, y = 0, ch;

	while ((ch = fgetc(file)) != EOF) {
		if (x == ASSETWIDTH) {
			y++;
			x = 0;
		}
		else {
			asset[y][x] = ch;
			x++;
		}
	}
}
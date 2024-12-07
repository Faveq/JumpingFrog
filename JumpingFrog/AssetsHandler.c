#include "functions.h"

void printBlocade(const int y, const int x, const Assets* asset)
{
	print(y, x, asset->blocadePrint);
}

void printCar(const int y, const int x, const Game* game, const int prevX, const int carId)
{
	Car* car = &game->cars[carId];

	if ((car->coordinates.x - car->direction) < GAMEBOARDWIDTH)
	{
		printRoad(car->coordinates.y * ASSETHEIGHT, prevX * ASSETWIDTH, &game->assets);
	}
	print(car->coordinates.y * ASSETHEIGHT, car->coordinates.x*ASSETWIDTH, game->assets.carPrint);
}

void printFrog(const int y, const int x, const Assets* asset)
{
	print(y, x, asset->frogPrint);
}

void printRoad(const int y, const int x, const Assets* printables)
{
	print(y, x, printables->roadPrint);
}

void printGrass(const int y, const int x, const Assets* printables)
{
	print(y, x, printables->grassPrint);
}

void print(const int y, const int x, const char asset[ASSETHEIGHT][ASSETWIDTH]) {
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

//mapping dowloaded assets into their arrays
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
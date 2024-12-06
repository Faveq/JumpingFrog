#include "functions.h"

int loadSettings(Game *game) {
    FILE* file = fopen("Settings.txt", "r");  
    if (file) {
        int timeLimit = 0;

        fscanf_s(file, "difficultyLevel = %d\n", &game->difficultyLevel);
        fscanf_s(file, "jumpCooldown = %d\n", &game->frog.jumpCooldown);
        fscanf_s(file, "blocade = %c\n", &game->obstacleCharacter, (int)sizeof(char));
        fscanf_s(file, "time = %d\n", &timeLimit);

        setTimeLimit(game, timeLimit);
        fclose(file);
        return 1;
    }   
    else
        return 0;
}   

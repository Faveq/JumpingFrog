#include "functions.h"
#include <stdio.h>

bool loadSettings(Game *game) {
    FILE* file = fopen("Settings.txt", "r");  
    if (file == NULL) {
        printf("Failed to open the file\n"); 
        return 0;
    }
    else {

        fscanf_s(file, "difficultyLevel = %d\n", &game->difficultyLevel);
        fscanf_s(file, "jumpCooldown = %f\n", &game->frog.jumpCooldown);
        fscanf_s(file, "blocade = %c\n", &game->obstacleCharacter, (int)sizeof(char));
        fscanf_s(file, "time = %d\n", &game->mainTimer.initialTime);

        fclose(file);
        return 1;
    }
}

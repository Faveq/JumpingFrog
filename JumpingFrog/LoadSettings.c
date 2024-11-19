#include "functions.h"
#include <stdio.h>

void loadSettings(struct Game *game) {
    FILE* file = fopen("Settings.txt", "r");  
    if (file == NULL) {
        printf("Failed to open the file\n"); 
    }
    else {

        fscanf_s(file, "difficultyLevel = %d\n", &game->difficultyLevel);
        fscanf_s(file, "jumpCooldown = %f\n", &game->frog.jumpCooldown);
        fscanf_s(file, "blocade = %c\n", &game->obstacleCharacter, (int)sizeof(char));
        fscanf_s(file, "time = %d\n", &game->time);

        fclose(file);
    }
}

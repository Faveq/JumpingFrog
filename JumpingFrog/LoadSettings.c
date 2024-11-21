#include "functions.h"

bool loadSettings(Game *game) {
    FILE* file = fopen("Settings.txt", "r");  
    if (file) {
        fscanf_s(file, "dL = %d\n", &game->difficultyLevel);
        fscanf_s(file, "jC = %f\n", &game->frog.jumpCooldown);
        fscanf_s(file, "b = %c\n", &game->obstacleCharacter, (int)sizeof(char));
        //fscanf_s(file, "time = %f\n", &game->mainTimer.initialTime);

        fclose(file);
        return 1;
    }   
    else
        return 0;
}

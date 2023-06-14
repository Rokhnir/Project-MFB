#include <stdio.h>
#include <stdlib.h>
#include "PPB/include/mapHandler.h"

int** map;
int mapHeight;
int mapWidth;

void createMap(const int levelIndex){

    char fileName[27] = "";
    snprintf(fileName, sizeof(fileName), "assets/maps/level_%d", levelIndex);

    FILE *file;
    file = fopen(fileName, "r");

    if(file == NULL) printf("[ERROR] > Could not open map file.");

    else {

        char line[100];

        fgets(line, sizeof(line), file);
        mapHeight = atoi(line);
        fgets(line, sizeof(line), file);
        mapWidth = atoi(line);

        map = (int**)malloc(mapHeight * sizeof(int*));
        for(int i = 0; i < mapHeight; i++){
            map[i] = (int*)malloc(mapWidth * sizeof(int));
        }

        for(int i = 0; i < mapHeight; i++){
            for(int j = 0; j < mapWidth; j++){
                fscanf(file, "%d", &map[i][j]);
            }
        }

    }
    
    fclose(file);
    file = NULL;

    return;

}

void freeMapMemory(void){

    for(int i = 0; i < mapHeight; i++){
        free(map[i]);
    }
    free(map);

    return;

}

int main(void){
    createMap(1);
    for(int i = 0; i < mapHeight; i++){
        for(int j = 0; j < mapWidth; j++){
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }
    freeMapMemory();
    return 0;
}
/* ----------------------------------------------------------- */
// INCLUDES
//
#include "../include/mapHandler.h" // self
#include <stdio.h> // snprintf | fopen | fgets | fscanf | fclose
#include <stdlib.h> // atoi | malloc | free
#include "../include/playerHandler.h" // initPlayer
#include "../../gfxlib/include/GfxLib.h" // messageDErreur

/* ----------------------------------------------------------- */
// GLOBAL VARS
//
int** map; // Tableay 2D conenant le layout de la map
int mapHeight; // Hauteur du tableau map
int mapWidth; // Largeur du tableau map


/* ----------------------------------------------------------- */
// FUNCTIONS
//

void createMap(const int levelIndex){

    char fileName[27] = "";
    snprintf(fileName, sizeof(fileName), "assets/maps/level_%d", levelIndex); // Construit un string contenant la variable de l'index du niveau

    FILE *file;
    file = fopen(fileName, "r");

    if(file != NULL){

        char line[100];

        fgets(line, sizeof(line), file); // Première ligne du fichier map : hauteur de la map
        mapHeight = atoi(line);
        fgets(line, sizeof(line), file); // Deuxième ligne du fichier map : largeur de la map
        mapWidth = atoi(line);

        map = (int**)malloc(mapHeight * sizeof(int*));
        for(int i = 0; i < mapHeight; i++){
            map[i] = (int*)malloc(mapWidth * sizeof(int)); // Création d'un tableau 2D avec les infos du fichier map
        }

        for(int i = 0; i < mapHeight; i++){
            for(int j = 0; j < mapWidth; j++){

                fscanf(file, "%d", &map[i][j]);

                if(map[i][j] < 0){ // Si < 0 dans le fichier : pos du spawn joueur
                    initPlayer(j, i, map[i][j] * -1); // La valeur de la case : direction (-1 = droite, -2 = haut...)
                    map[i][j] = 0; // Les coordonnées du spawn joueur d'apparaissent pas dans le tableau 2D
                }

            }
        }

        fclose(file);
        file = NULL;

    }
    else messageDErreur("[mapHandler] > Could not open map file."); // Printf impossible avec une fenêtre glut en process

    return;

}

void freeMapMemory(void){

    for(int i = 0; i < mapHeight; i++){
        free(map[i]);
    }
    free(map);

    return;

}
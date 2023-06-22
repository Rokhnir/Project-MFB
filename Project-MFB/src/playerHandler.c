/* ----------------------------------------------------------- */
// INCLUDES
//
#include "../include/playerHandler.h" // self
#include <math.h> // cos | sin
#include "../include/rayCasting.h" // toRads | fixAngle
#include "../include/main.h" // fps
#include "../include/mapHandler.h"
// map

/* ----------------------------------------------------------- */
// GLOBAL VAR
//
// Structure contenant les données du joueur
extern PlayerStr player;
/* ----------------------------------------------------------- */
// FUNCTIONS
//

void initPlayer(int posX, int posY, int dir){

    player.posx = (float){posX << 6} + 32.; // << 6 = division par 64 (les cases ont une taille figurative de 64)
    player.posy = (float){posY << 6} + 32.; // + 32 : moitiée d'un case (centre le joueur sur la case)

    float direction[4] = {0., 90., 180., 270.};
    player.dirA = direction[dir-1];

    player.dirX = cos(toRads(player.dirA));
    player.dirY = -sin(toRads(player.dirA));

}

void movePlayer(char direction){

    float newPosX, newPosY;

    switch(direction){
        case 'z':
            newPosX = player.posx + player.dirX * 0.4 * fps;
            newPosY = player.posy + player.dirY * 0.4 * fps;
            if(map[(int){player.posy} >> 6][(int){newPosX} >> 6] < 1) player.posx = newPosX;
            if(map[(int){newPosY} >> 6][(int){player.posx} >> 6] < 1) player.posy = newPosY;
            break;
        case 'q':
            player.dirA += 0.2 * fps;
            player.dirA = fixAngle(player.dirA);
            player.dirX = cos(toRads(player.dirA));
            player.dirY = -sin(toRads(player.dirA));
            break;
        case 's':
            newPosX = player.posx - player.dirX * 0.4 * fps;
            newPosY = player.posy - player.dirY * 0.4 * fps;
            if(map[(int){player.posy} >> 6][(int){newPosX} >> 6] < 1) player.posx = newPosX;
            if(map[(int){newPosY} >> 6][(int){player.posx} >> 6] < 1) player.posy = newPosY;
            break;
        case 'd':
            player.dirA -= 0.2 * fps;
            player.dirA = fixAngle(player.dirA);
            player.dirX = cos(toRads(player.dirA));
            player.dirY = -sin(toRads(player.dirA));
            break;
    }

}
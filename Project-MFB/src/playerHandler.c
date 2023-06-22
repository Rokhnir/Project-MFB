/* ----------------------------------------------------------- */
// INCLUDES
//
#include "../include/playerHandler.h" // self
#include <math.h> // cos | sin
#include "../include/rayCasting.h" // toRads | fixAngle
#include "../include/main.h" // fps
#include "../include/mapHandler.h" // map

/* ----------------------------------------------------------- */
// GLOBAL VAR
//
Player p; // Structure contenant les données du joueur

/* ----------------------------------------------------------- */
// FUNCTIONS
//

void initPlayer(int posX, int posY, int dir){

    p.posX = (float){posX << 6} + 32.; // << 6 = division par 64 (les cases ont une taille figurative de 64)
    p.posY = (float){posY << 6} + 32.; // + 32 : moitiée d'un case (centre le joueur sur la case)

    float direction[4] = {0., 90., 180., 270.};
    p.dirA = direction[dir-1];

    p.dirX = cos(toRads(p.dirA));
    p.dirY = -sin(toRads(p.dirA));

}

void movePlayer(char direction){

    float newPosX, newPosY;

    switch(direction){
        case 'z':
            newPosX = p.posX + p.dirX * 0.2 * fps;
            newPosY = p.posY + p.dirY * 0.2 * fps;
            if(!map[(int){p.posY} >> 6][(int){newPosX} >> 6]) p.posX = newPosX;
            if(!map[(int){newPosY} >> 6][(int){p.posX} >> 6]) p.posY = newPosY;
            break;
        case 'q':
            p.dirA += 0.2 * fps;
            p.dirA = fixAngle(p.dirA);
            p.dirX = cos(toRads(p.dirA));
            p.dirY = -sin(toRads(p.dirA));
            break;
        case 's':
            newPosX = p.posX - p.dirX * 0.2 * fps;
            newPosY = p.posY - p.dirY * 0.2 * fps;
            if(!map[(int){p.posY} >> 6][(int){newPosX} >> 6]) p.posX = newPosX;
            if(!map[(int){newPosY} >> 6][(int){p.posX} >> 6]) p.posY = newPosY;
            break;
        case 'd':
            p.dirA -= 0.2 * fps;
            p.dirA = fixAngle(p.dirA);
            p.dirX = cos(toRads(p.dirA));
            p.dirY = -sin(toRads(p.dirA));
            break;
    }

}
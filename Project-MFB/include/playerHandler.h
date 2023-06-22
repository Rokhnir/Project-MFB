#ifndef PLAYERHANDLER_H
#define PLAYERHANDLER_H

#include "struct.h"
/*typedef struct{
    float posX;
    float posY;
    float dirA;
    float dirX;
    float dirY;
}Player;*/

/*extern Player p;*/

void initPlayer(int posX, int posY, int dir);
void movePlayer(char direction);

#endif
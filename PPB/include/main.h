#ifndef MAIN_H
#define MAIN_H

typedef struct{
    int z,q,s,d;
}KeysStruct;

typedef struct{
    float posX;
    float posY;
    float dirX;
    float dirY;
    float dirA;
}Player;

void keyDown(unsigned char key);
void keyUp(unsigned char key, int x, int y);

#endif
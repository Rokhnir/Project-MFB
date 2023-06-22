#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>

extern int screenHeight;
extern int screenWidth;
extern float fps;

typedef struct{
    int z,q,s,d;
}KeysStruct;

void keyDown(unsigned char key);
void keyUp(unsigned char key, int x, int y);

/*
 char Music[] = {"../../assets/music/uwu.wav"};
*/
/* bool loopThreadStarted = false;
 bool loopThreadStarted2 = false;
 bool loopThreadSond = false;

 bool arret = false;
 char cheatCode[10] = {};*/

#endif
#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include "struct.h"

extern int screenHeight;
extern int screenWidth;
extern float fps;

typedef struct{
    int z,q,s,d;
}KeysStruct;

void keyDown(unsigned char key);
void keyUp(unsigned char key, int x, int y);
Ennemie initEnnemie(int x, int y);
void iaEnnemie(Ennemie a);
void arret_brutal_music(void);
void play_sound(const char *filename);
void play_music(void *arg);
void *play_sound_parallel(void *filename);
void ui();
void newHUD();
void *loopThread(void *arg);
void gameStart(void);

void attacks(Ennemie *a);
int defenses(Ennemie a);
/*
 char Music[] = {"../../assets/music/uwu.wav"};
*/
/* bool loopThreadStarted = false;
 bool loopThreadStarted2 = false;
 bool loopThreadSond = false;

 bool arret = false;
 char cheatCode[10] = {};*/

#endif
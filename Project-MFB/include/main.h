#ifndef MAIN_H
#define MAIN_H

extern int screenHeight;
extern int screenWidth;
extern float fps;

typedef struct{
    int z,q,s,d;
}KeysStruct;

void keyDown(unsigned char key);
void keyUp(unsigned char key, int x, int y);

#endif
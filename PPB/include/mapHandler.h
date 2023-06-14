#ifndef MAPHANDLER_H
#define MAPHANDLER_H

extern int** map;
extern int mapHeight;
extern int mapWidth;

void fileReader(const int levelIndex);
void freeMapMemory(void);

#endif
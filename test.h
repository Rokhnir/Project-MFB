//
// Created by kyli8 on 12/06/2023.
//

#ifndef WORKSPACE_TEST_H
#define WORKSPACE_TEST_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "gfxlib/include/GfxLib.h"
#include "struct.h"

#define screenWidth 1920
#define screenHeight 1080
#define mapWidth 24
#define mapHeight 24


void gestionEvenement(EvenementGfx evenement);
void loop();
int worldMap[mapWidth][mapHeight];
void afficheAmmo();
void attacks(Ennemie *a);
int defenses(Ennemie a);

double pX;
double pY;
double planeX = 0;
double planeY = screenWidth;
double dirX;
double dirY;
double pvecX;
double pvecY;
double dvecX;
double dvecY;
double angle;
double oldDirX;
double oldPlaneX;

Weapon dupStep = {"Dupstep Gun",0,6,10,900,0};
Weapon rifle = {"Rifle",0 ,30 ,5 ,250, 0};
Weapon gun = {"Gun",0 ,12 ,7 ,700, 0};




#endif //WORKSPACE_TEST_H

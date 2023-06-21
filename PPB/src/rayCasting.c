#include <stdlib.h>
#include <math.h>
#include "../include/rayCasting.h"
#include "../../gfxlib/include/GfxLib.h"
#include <stdio.h>

extern int** map;
extern int mapHeight;
extern int mapWidth;
extern int screenHeight;
extern int screenWidth;

typedef struct Player{
    float posX;
    float posY;
    float dirX;
    float dirY;
    float dirA;
}Player;
Player p = { .posX = 128, .posY = 128, .dirX = -0.44807361612, .dirY = -0.8939966636, .dirA = 90};

float toRads(int angle){
    return (float){angle * M_PI / 180.};
}

int fixAngle(int angle){
    if(angle > 360) angle -= 360;
    else if(angle < 0) angle += 360;
    return angle;
}

float dda(int rayA){

    int depth = 0, mapX, mapY;
    float xOffset = 0., yOffset = 0., rayTan = 0., distX = 100000.f, distY = 100000.f, rayX = 0., rayY = 0.;

    //--- Vertical Check ---
    rayTan = tan(toRads(fixAngle(p.dirA + 30)));

    if(cos(toRads(rayA)) > 0.001){
        rayX = (((int)p.posX >> 6) << 6) + 64;
        rayY = (p.posX - rayX) * rayTan + p.posY;
        xOffset = 64;
        yOffset = -xOffset * rayTan;
    }
    else if(cos(toRads(rayA)) < 0.001){
        rayX = (((int)p.posX >> 6) << 6) - 0.0001;
        rayY = (p.posX - rayX) * rayTan + p.posY;
        xOffset = -64;
        yOffset = -xOffset * rayTan;
    }
    else {
        rayX += xOffset;
        rayY += yOffset;
        depth = mapHeight;
    }
    while(depth < mapHeight){
        mapX = (int)rayX >> 6;
        mapY = (int)rayY >> 6;
        if(map[mapX][mapY] > 0){
            depth = mapHeight;
            distX = cos(toRads(rayA)) * (rayX - p.posX) - sin(toRads(rayA)) * (rayY - p.posY);
        }
        else {
            rayX += xOffset;
            rayY += yOffset;
            depth++;
        }
    }
    
    //--- Vertical Check ---
    depth = 0;
    rayTan = 1./rayTan;

    if(cos(toRads(rayA)) > 0.001){
        rayX = (p.posY - rayY) * rayTan + p.posX;
        rayY = (((int)p.posX >> 6) << 6) - 0.0001;
        xOffset = -64;
        yOffset = -yOffset * rayTan;
    }
    else if(cos(toRads(rayA)) < 0.001){
        rayX = (p.posY - rayY) * rayTan + p.posX;
        rayY = (((int)p.posX >> 6) << 6) + 64;
        xOffset = 64;
        yOffset = -yOffset * rayTan;
    }
    else {
        rayX += xOffset;
        rayY += yOffset;
        depth = mapWidth;
    }
    while(depth < mapWidth){
        mapX = (int)rayX >> 6;
        mapY = (int)rayY >> 6;
        if(map[mapX][mapY] > 0){
            depth = mapHeight;
            distY = cos(toRads(rayA)) * (rayX - p.posX) - sin(toRads(rayA)) * (rayY - p.posY);
        }
        else {
            rayX += xOffset;
            rayY += yOffset;
            depth++;
        }
    }

    //--- End ---
    float distance = fminf(distX, distY);
    int side = (distance == distX) ? -1 : 1;
    float returnValue = distance * side * cos(toRads(fixAngle(p.dirA - rayA)));
    return returnValue;
}

void drawWall(float distance, const int widthIndex){
    epaisseurDeTrait(1.f);
    float value = 255.;
    if(distance < 0){
        distance *= -1;
        value = 155.;
    }
    couleurCourante(value, 0 , 0);
    int lineHeight = (64 * screenHeight) / distance;
    if(lineHeight > screenHeight) lineHeight = screenHeight;
    
    int drawStart = -lineHeight / 2 + screenHeight / 2;
    if(drawStart < 0)drawStart = 0;
    int drawEnd = lineHeight / 2 + screenHeight / 2;
    if(drawEnd >= screenHeight)drawEnd = screenHeight - 1;

    ligne(widthIndex, drawStart, widthIndex, drawEnd);
}

void rayCasting(void){
    for(int i = 0; i < 60; i++){
        drawWall(dda(i), i);
    }
    return;
}
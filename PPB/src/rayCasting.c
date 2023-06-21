#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/rayCasting.h"
#include "../../gfxlib/include/GfxLib.h"

extern int** map;
extern int mapHeight;
extern int mapWidth;
extern int screenHeight;
extern int screenWidth;

typedef struct{
    float posX;
    float posY;
    float dirX;
    float dirY;
    float dirA;
}Player;
Player p = { .posX = 150, .posY = 400, .dirX = 0, .dirY = -1, .dirA = 90};

float toRads(float angle){
    return angle * M_PI / 180.;
}

float fixAngle(float angle){
    if(angle >= 360) angle -= 360;
    else if(angle < 0) angle += 360;
    return angle;
}

void rayCasting(void){

    float rayA = fixAngle(p.dirA + 30);

    for(int ray = 0; ray < 120; ray++){

        float tanRayA = tan(toRads(rayA));
        float distH = dda('H', rayA, 1./tanRayA);
        float distV = dda('V', rayA, tanRayA);

        couleurCourante(255,0,0);
        if(distV < distH){
            couleurCourante(155,0,0);
        }

        int lineHeight = (64 * screenHeight) / (distH * cos(toRads(fixAngle(p.dirA - rayA))));

        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if(drawStart < 0)drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if(drawEnd >= screenHeight)drawEnd = screenHeight - 1;

        epaisseurDeTrait(screenWidth/120.);
        ligne(ray * screenWidth/120., drawStart, ray * screenWidth/120., drawEnd);

        rayA = fixAngle(rayA - 0.5);

    }




}

float dda(const char axe, const float rayA, const float tanRayA){

    float returnValue = 100000, rayX = 0., rayY = 0., xOffset = 0., yOffset = 0.;
    int depth = 0;

    if(axe == 'H' && sin(toRads(rayA)) > 0.001) {
        rayY = (((int)p.posY >> 6) << 6) - 0.0001;
        rayX = (p.posY - rayY) * tanRayA + p.posX;
        yOffset = -64;
        xOffset = -yOffset * tanRayA;
    }
    else if(axe == 'V' && cos(toRads(rayA)) > 0.001) {
        rayX = (((int)p.posY >> 6) << 6) + 64;
        rayY = (p.posX - rayX) * tanRayA + p.posY;
        xOffset = 64;
        yOffset = -xOffset * tanRayA;
    }
    else if(axe == 'H' && sin(toRads(rayA)) < 0.001){
        rayY = (((int)p.posY >> 6) << 6) + 64;
        rayX = (p.posY - rayY) * tanRayA + p.posX;
        yOffset = 64;
        xOffset = -yOffset * tanRayA;
    }
    else if(axe == 'V' && cos(toRads(rayA)) < 0.001){
        rayX = (((int)p.posY >> 6) << 6) - 0.0001;
        rayY = (p.posX - rayX) * tanRayA + p.posY;
        xOffset = -64;
        yOffset = -xOffset * tanRayA;
    }
    else {
        rayX = p.posX;
        rayY = p.posY;
        depth = 8;
    }
    
    while(depth < 8){
        int mapX = (int){rayY} >> 6;
        int mapY = (int){rayX} >> 6;
        if(0 < mapX && mapX < mapHeight && 0 < mapY && mapY < mapWidth && map[mapX][mapY] > 0){
            depth = 8;
            returnValue = cos(toRads(rayA)) * (rayX - p.posX) - sin(toRads(rayA)) * (rayY - p.posY);
        }
        else{
            rayX += xOffset;
            rayY += yOffset;
            depth++;
        }
    }

    return returnValue;

}
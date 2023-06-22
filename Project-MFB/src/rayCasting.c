/* ----------------------------------------------------------- */
// INCLUDES
//
#include "../include/rayCasting.h" // self
#include <math.h> // M_PI | tan | sin | cos
#include "../include/playerHandler.h" // p.dirA | p.posX | p.posY
#include "../../gfxlib/include/GfxLib.h" // couleurCourante | ligne
#include "../include/main.h" // screenHeight | screenWidth
#include "../include/mapHandler.h" // mapWidth | mapHeight | map


/* ----------------------------------------------------------- */
// FUNCTIONS
//

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

        int colorH = 0, colorV = 0;
        float distH = dda('H', rayA, 1./tanRayA, &colorH);
        float distV = dda('V', rayA, tanRayA, &colorV);

        int color = 0, shade = 1;

        if(distV < distH){
            shade = 2;
            color = colorV;
            distH = distV;
        }
        else color = colorH;

        setColor(color, shade);

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

float dda(const char axe, const float rayA, const float tanRayA, int *color){

    float returnValue = 100000., rayX = 0., rayY = 0., xOffset = 0., yOffset = 0.;
    int depth = 0, maxDepth = (axe == 'H') ? mapHeight : mapWidth;

    if(axe == 'H' && sin(toRads(rayA)) > 0.001) { // HAUT
        rayY = (((int)p.posY >> 6) << 6) - 0.0001;
        rayX = (p.posY - rayY) * tanRayA + p.posX;
        yOffset = -64;
        xOffset = -yOffset * tanRayA;
    }
    else if(axe == 'V' && cos(toRads(rayA)) > 0.001) { // GAUCHE
        rayX = (((int)p.posX >> 6) << 6) + 64;
        rayY = (p.posX - rayX) * tanRayA + p.posY;
        xOffset = 64;
        yOffset = -xOffset * tanRayA;
    }
    else if(axe == 'H' && sin(toRads(rayA)) < -0.001){ // BAS
        rayY = (((int)p.posY >> 6) << 6) + 64;
        rayX = (p.posY - rayY) * tanRayA + p.posX;
        yOffset = 64;
        xOffset = -yOffset * tanRayA;
    }
    else if(axe == 'V' && cos(toRads(rayA)) < -0.001){ // DROITE
        rayX = (((int)p.posX >> 6) << 6) - 0.0001;
        rayY = (p.posX - rayX) * tanRayA + p.posY;
        xOffset = -64;
        yOffset = -xOffset * tanRayA;
    }
    else {
        rayX = p.posY;
        rayY = p.posX;
        depth = maxDepth;
    }

    while(depth < maxDepth){
        
        int mapX = (int){rayY} >> 6;
        int mapY = (int){rayX} >> 6;

        if(0 <= mapX && mapX < mapWidth && 0 <= mapY && mapY < mapHeight && map[mapX][mapY] > 0){
            depth = maxDepth;
            *color = map[mapX][mapY];
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

void setColor(const int color, const int shade){

    switch(color){
        case 1:
            couleurCourante(255./shade, 0, 0);
            break;
        case 2:
            couleurCourante(0, 255./shade, 0);
            break;
        case 3:
            couleurCourante(0, 0, 255./shade);
            break;
        case 4:
            couleurCourante(255./shade, 255./shade, 255./shade);
            break;
        case 5:
            couleurCourante(255./shade, 0 ,255./shade);
            break;
    }

}
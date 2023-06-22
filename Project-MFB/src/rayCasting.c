/* ----------------------------------------------------------- */
// INCLUDES
//
#include "../include/rayCasting.h" // self
#include <math.h> // M_PI | tan | sin | cos
#include "../include/playerHandler.h" // p.dirA | p.posX | p.posY
#include "../../gfxlib/include/GfxLib.h" // couleurCourante | ligne
#include "../include/main.h" // 640 | 960
#include "../include/mapHandler.h" // mapWidth | mapHeight | map
#include "../../gfxlib/include/BmpLib.h" // DonneesImageRGB | lisBMPRGB | ecrisImage
#include <stdio.h> // NULL

/* ----------------------------------------------------------- */
// GLOBAL VARS
//
static int wallDepth[240];
extern PlayerStr player;


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

void rayCasting(DonneesImageRGB *wallTexture){

    float rayA = fixAngle(player.dirA + 30);

    for(int ray = 0; ray < 240; ray++){

        float tanRayA = tan(toRads(rayA));

        float vRayX = 0., vRayY = 0., hRayX = 0., hRayY = 0.;
        float distH = dda('H', rayA, 1./tanRayA, &hRayX, &hRayY);
        float distV = dda('V', rayA, tanRayA, &vRayX, &vRayY);

        int shade = 1;

        if(distV < distH){
            shade = 2;
            hRayX = vRayX;
            hRayY = vRayY;
            distH = distV;
        }

        wallDepth[ray] = distH;

        int lineHeight = (64 * 640) / (distH * cos(toRads(fixAngle(player.dirA - rayA))));

        float textureStep = 32./(float)lineHeight;
        float textureYOffset = 0;
        if(lineHeight > 640){
            textureYOffset = (lineHeight - 640)/2.;
            lineHeight = 640;
        }
        int lineOffset = 320 - (lineHeight >> 1) + (screenHeight - 640) / 2;
        float textureY = textureYOffset * textureStep;
        float textureX;
        if(shade == 1){
            textureX = (int){hRayX / 2} % 32;
            if(rayA > 180) textureX = 31 - textureX;
        }
        else{
            textureX = (int){hRayY / 2} % 32;
            if(rayA > 90 && rayA < 270) textureX = 31 - textureX;
        }

        for(int j = 0; j < lineHeight; j++){

            int pixel = ((int)textureY * 32 + (int)textureX) * 3;
            setColor(pixel, shade, wallTexture);
            epaisseurDeTrait(1.);
            rectangle((screenWidth - 960) / 2 + (ray * 4), j + lineOffset, (screenWidth - 960) / 2 + (ray * 4) + 4, j + lineOffset + 4);
            textureY += textureStep;

        }

        rayA = fixAngle(rayA - 0.25);

    }

}

float dda(const char axe, const float rayA, const float tanRayA, float *rRayX, float *rRayY){

    float returnValue = 100000., rayX = 0., rayY = 0., xOffset = 0., yOffset = 0.;
    int depth = 0, maxDepth = (axe == 'H') ? mapHeight : mapWidth;

    if(axe == 'H' && sin(toRads(rayA)) > 0.001) { // HAUT
        rayY = (((int)player.posy >> 6) << 6) - 0.0001;
        rayX = (player.posy - rayY) * tanRayA + player.posx;
        yOffset = -64;
        xOffset = -yOffset * tanRayA;
    }
    else if(axe == 'V' && cos(toRads(rayA)) > 0.001) { // GAUCHE
        rayX = (((int)player.posx >> 6) << 6) + 64;
        rayY = (player.posx - rayX) * tanRayA + player.posy;
        xOffset = 64;
        yOffset = -xOffset * tanRayA;
    }
    else if(axe == 'H' && sin(toRads(rayA)) < -0.001){ // BAS
        rayY = (((int)player.posy >> 6) << 6) + 64;
        rayX = (player.posy - rayY) * tanRayA + player.posx;
        yOffset = 64;
        xOffset = -yOffset * tanRayA;
    }
    else if(axe == 'V' && cos(toRads(rayA)) < -0.001){ // DROITE
        rayX = (((int)player.posx >> 6) << 6) - 0.0001;
        rayY = (player.posx - rayX) * tanRayA + player.posy;
        xOffset = -64;
        yOffset = -xOffset * tanRayA;
    }
    else {
        rayX = player.posy;
        rayY = player.posx;
        depth = maxDepth;
    }

    while(depth < maxDepth){

        int mapX = (int){rayY} >> 6;
        int mapY = (int){rayX} >> 6;

        if(0 <= mapX && mapX < mapWidth && 0 <= mapY && mapY < mapHeight && map[mapX][mapY] > 0){
            depth = maxDepth;
            *rRayX = rayX;
            *rRayY = rayY;
            returnValue = cos(toRads(rayA)) * (rayX - player.posx) - sin(toRads(rayA)) * (rayY - player.posy);
        }
        else{
            rayX += xOffset;
            rayY += yOffset;
            depth++;
        }

    }

    return returnValue;

}

int setColor(const int pixelIndex, const int shade, const DonneesImageRGB *wallTexture){

    unsigned char *pixelsValues = wallTexture->donneesRGB;

    float red = pixelsValues[pixelIndex+2] / (float)shade;
    float green = pixelsValues[pixelIndex+1] / shade;
    float blue = pixelsValues[pixelIndex+0] /shade;

    couleurCourante(red, green, blue);

    if(red == 255 && green == 0 && blue == 255) return 1;
    else return 0;

}

void drawEnemy(float posX, float posY, DonneesImageRGB *enemyTexture){

    float screenPosX = posX - player.posx;
    float screenPosY = posY - player.posy;

    float distX = screenPosY * cos(toRads(player.dirA)) + screenPosX * sin(toRads(player.dirA));
    float distY = screenPosX * cos(toRads(player.dirA)) - screenPosY * sin(toRads(player.dirA));
    screenPosX = distX; screenPosY = distY;

    screenPosX = (screenPosX * 108. / screenPosY) + (240 >> 1);
    screenPosY = (20 * 108. / screenPosY) + (80 >> 1);

    int scale = 32 * 400 / distY;
    if(scale < 0) scale = 0;
    if(scale > 240) scale = 240;

    float textureX = 0., textureY = 31., textureStepX = 31.5 / (float)scale, textureStepY = 32. / (float)scale;

    for(int x = screenPosX - scale / 2; x < screenPosX + scale / 2; x++){

        textureY = 31;

        for(int y = 0; y < scale; y++){

            if(x > 0 && x < 240 && distY < wallDepth[x]){
                int pixel = ((int) textureY * 32 + (int)textureX) * 3;
                if(!setColor(pixel, 1, enemyTexture)){

                    float y1 = screenPosY * 8 - y * 4 + (screenHeight - 640) / 2;
                    float y2 = screenPosY * 8 - y * 4 + 4 + (screenHeight - 640) / 2;

                    if(y1 > (screenHeight - 640) / 2 && y2 < (screenHeight - 640) / 2 + 640){
                        epaisseurDeTrait(1.);
                        rectangle((screenWidth - 960) / 2 + (x * 4), y1, (screenWidth - 960) / 2 + (x * 4) + 4, y2);
                    }

                }
                textureY -= textureStepY;
                if(textureY < 0) textureY=0;
            }

        }
        textureX += textureStepX;

    }

}
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

static float returnValue = 100000., rayX = 0., rayY = 0., xOffset = 0., yOffset = 0.;
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

        int lineHeight = (64 * 640) / (distH * cos(toRads(fixAngle(p.dirA - rayA))));

        int drawStart = (-lineHeight / 2 + 640 / 2) + (screenHeight - 640) / 2;
        if(drawStart < (screenHeight - 640) / 2) drawStart = (screenHeight - 640) / 2;
        int drawEnd = (lineHeight / 2 + 640 / 2) + (screenHeight - 640) / 2;
        if(drawEnd >= 640 + (screenHeight - 640) / 2) drawEnd = (640 + (screenHeight - 640) / 2) - 1;

        float ty = drawStart*64./lineHeight;
        float tx;

        if(shade==1) tx=(int){rayX/2}%64;
        else tx=(int){rayY/2}%64;

        for(int j = 0; j < lineHeight; j++){
            int pixel = ((int){ty}*64+(int){tx})*3;
            setColor(pixel, shade, wallTexture);
            epaisseurDeTrait(960/120.);
            point((screenWidth - 960) / 2 + (ray * 960/120.), j+drawStart);
        }

        //ligne((screenWidth - 960) / 2 + (ray * 960/120.), drawStart, (screenWidth - 960) / 2 + (ray * 960/120.), drawEnd);

        rayA = fixAngle(rayA - 0.5);

    }

}

float dda(const char axe, const float rayA, const float tanRayA, int *color){

    
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

void setColor(const int pixelIndex, const int shade, const DonneesImageRGB *wallTexture){

/*if(wallTextureexture != NULL) {
        char test = texture->donneesRGB[0];
        char fileName[27] = "";
        snprintf(fileName, sizeof(fileName), "%d", test);
        messageDErreur(fileName);
    }*/

    unsigned char *test = wallTexture->donneesRGB;

    float red = test[pixelIndex+0]/shade;
    float green = test[pixelIndex+1]/shade;
    float blue = test[pixelIndex+2]/shade;

    couleurCourante(red, green, blue);

}
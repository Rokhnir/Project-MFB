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

        int lineHeight = (64 * 640) / (distH * cos(toRads(fixAngle(p.dirA - rayA))));

        float ty_step = 32./(float)lineHeight;
        float ty_off=0;
        if(lineHeight>640){ty_off=(lineHeight-640)/2.; lineHeight=640;}
        int lineOff = 320 -(lineHeight>>1) + (screenHeight - 640) / 2;

        float ty=ty_off*ty_step;
        float tx;
        if(shade==1){
            tx=(int){hRayX/2}%32;
            if(rayA>180) tx = 31-tx;
        }
        else{
            tx=(int){hRayY/2}%32;
            if(rayA>90 && rayA<270) tx = 31 - tx;
        }

        for(int j = 0; j < lineHeight; j++){
            int pixel = ((int)ty*32+(int)tx)*3;
            setColor(pixel, shade, wallTexture);
            epaisseurDeTrait(1.);
            rectangle((screenWidth - 960) / 2 + (ray * 8), j+lineOff, (screenWidth - 960) / 2 + (ray * 8) + 8, j+lineOff+8);
            ty+=ty_step;
        }

        rayA = fixAngle(rayA - 0.5);

    }

}

float dda(const char axe, const float rayA, const float tanRayA, float *rRayX, float *rRayY){

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
            *rRayX = rayX;
            *rRayY = rayY;
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

    unsigned char *pixelsValues = wallTexture->donneesRGB;

    float red = pixelsValues[pixelIndex+2] / (float)shade;
    float green = pixelsValues[pixelIndex+1] / shade;
    float blue = pixelsValues[pixelIndex+0] /shade;

    couleurCourante(red, green, blue);

}
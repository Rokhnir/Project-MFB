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

        int lineHeight = (64 * 640) / (distH * cos(toRads(fixAngle(p.dirA - rayA))));

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

int setColor(const int pixelIndex, const int shade, const DonneesImageRGB *wallTexture){

    unsigned char *pixelsValues = wallTexture->donneesRGB;

    float red = pixelsValues[pixelIndex+2] / (float)shade;
    float green = pixelsValues[pixelIndex+1] / shade;
    float blue = pixelsValues[pixelIndex+0] /shade;

    couleurCourante(red, green, blue);

    if(red == 255 && green == 0 && blue == 255) return 1;
    else return 0;

}

void drawEnemy(DonneesImageRGB *enemyTexture){

    float posX = 0., posY = 0;

    for(int i  = 0; i < mapHeight; i++){
        for(int j = 0; j < mapWidth; j++){
            if(map[i][j] == -1) {
                posX = j * 64. + 32.;
                posY = i * 64. + 32.;
            }
        }
    }

    float sx = posX - p.posX;
    float sy = posY - p.posY;

    /*float screenPosX = distY * cos(toRads(p.dirA)) + distX * sin(toRads(p.dirA));
    float screenPosY = distX * cos(toRads(p.dirA)) - distY * sin(toRads(p.dirA));

    screenPosX = (screenPosX * 108. / screenPosY) + (240 / 2);
    screenPosY = (20 * 108. / screenPosY);

    int scale = 32 * 80 / (distX * cos(toRads(p.dirA)) - distY * sin(toRads(p.dirA)));
    if(scale < 0) scale = 0;
    if(scale > 240) scale = 240;

    float textureX = 0. , textureY = 31., textureStepX = 31.5 / (float)scale, textureStepY = 32. / (float)scale;

    for(int x = screenPosX - scale / 2; x < screenPosX + scale; x++){

        textureY = 31;

        for(int y = 0; y < scale; y++){

            if(x > 0 && x < 240){
                
                int pixel = ((int)textureY * 32 + (int)textureX) * 3;

                if(!setColor(pixel, 1, enemyTexture)){
                    epaisseurDeTrait(1.);
                    rectangle((screenWidth - 960) / 2 + (x * 4), screenPosY * 4 - y * 4, (screenWidth - 960) / 2 + (x * 4) + 4, screenPosY * 4 - y * 4 + 4);
                })

                textureY -= textureStepY;
                if(textureY < 0) textureY = 0;

            }

        }

        textureX += textureStepX;

    }*/


  float CS=cos(toRads(p.dirA)), SN=sin(toRads(p.dirA)); //rotate around origin
  float a=sy*CS+sx*SN; 
  float b=sx*CS-sy*SN; 
  sx=a; sy=b;

  sx=(sx*108.0/sy)+(240/2); //convert to screen x,y
  sy=(20*108.0/sy)+( 80/2);

  int scale=32*400/b;   //scale sprite based on distance
  if(scale<0){ scale=0;} if(scale>240){ scale=240;}  

  //texture
  float t_x=0, t_y=31, t_x_step=31.5/(float)scale, t_y_step=32.0/(float)scale;

  for(int x=sx-scale/2;x<sx+scale/2;x++)
  {
   t_y=31;
   for(int y=0;y<scale;y++)
   {
    if(x>0 && x<240 && b<wallDepth[x])
    {
     int pixel=((int)t_y*32+(int)t_x)*3;
    if(!setColor(pixel, 1, enemyTexture)){
        epaisseurDeTrait(1.);
        rectangle((screenWidth - 960) / 2 + (x * 4), sy*8-y*4 + (screenHeight - 640) / 2, (screenWidth - 960) / 2 + (x * 4) + 4, sy*8-y*4 + 4 + (screenHeight - 640) / 2);
    }
     t_y-=t_y_step; if(t_y<0){ t_y=0;}
    }
   }
   t_x+=t_x_step;
  }
 

}
#ifndef RAYCASTING_H
#define RAYCASTING_H

#include "../../gfxlib/include/BmpLib.h"

float toRads(float angle);
float fixAngle(float angle);
void rayCasting(DonneesImageRGB *wallTexture);
float dda(const char axe, const float rayA, const float tanRayA, float *rRayX, float *rRayY);
int setColor(const int pixelIndex, const int shade, const DonneesImageRGB *wallTexture);
void drawEnemy(float posX, float posY, DonneesImageRGB *enemyTexture);

#endif
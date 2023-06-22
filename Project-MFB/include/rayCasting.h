#ifndef RAYCASTING_H
#define RAYCASTING_H

#include "../../gfxlib/include/BmpLib.h"

float toRads(float angle);
float fixAngle(float angle);
void rayCasting(DonneesImageRGB *wallTexture);
float dda(const char axe, const float rayA, const float tanRayA, float *rRayX, float *rRayY);
void setColor(const int pixelIndex, const int shade, const DonneesImageRGB *wallTexture);
void rayTir(int coor[2]);
void dda2(const char axe, const float rayA, const float tanRayA, int tab[2]);

#endif
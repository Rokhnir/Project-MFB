#ifndef RAYCASTING_H
#define RAYCASTING_H

float toRads(float angle);
float fixAngle(float angle);
void rayCasting(void);
float dda(const char axe, const float rayA, const float tanRayA, int *color);
void setColor(const int color, const int shade);

#endif
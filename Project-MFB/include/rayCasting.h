#ifndef RAYCASTING_H
#define RAYCASTING_H

float toRads(float angle);
float fixAngle(float angle);
float dda(const char axe, const float rayA, const float tanRayA);
void rayCasting(void);

#endif
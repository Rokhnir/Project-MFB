#include <math.h>
#include <data.h>
#include "../include/data.h"
#include "../include/rayCasting.h"
#define PI 3.14159265

float toRadians(int angle){
    return (float){angle * M_PI / 180.};
}

int fixAngle(int angle){
    if(angle > 360) angle -= 360;
    else if(angle < 0) angle += 360;
    return angle;
}




void drawWall(float wallHeight)

void rayCasting(void){}
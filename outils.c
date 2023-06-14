//
// Created by kyli8 on 12/06/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "struct.h"
#include "outils.h"


double Rdistance(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));  //formule de distance entre deux point en rond
}


void iaennemie(Ennemie a) {
    double dist = Rdistance(a.posx, a.posy, player.posx, player.posy);
    if (dist <= a.rangeView) {
        if (dist <= a.rangeAttack) {
            player.defense(a);
        }
        else {
            double x = player.posx - a.posx;
            double y = player.posy - a.posy;
            if (fabs(x) > fabs(y)) {
                if (x >= 0) {
                    if (worldMap[(a.posx + a.speed) % 64][a.posy] != 0) {
                        if (y >= 0) {
                            a.posy += a.speed;
                        }
                        else {
                            a.posy -= a.speed;
                        }
                    }
                    a.posx += a.speed;
                }
                else {
                    if (worldMap[(a.posx - a.speed) % 64][a.posy % 64] != 0) {
                        if (y >= 0) {
                            a.posy += a.speed;
                        }
                        else {
                            a.posy -= a.speed;
                        }
                    }
                    a.posx -= a.speed;
                }
            }
            else {
                if (y >= 0) {
                    if (worldMap[a.posx % 64][(a.posy + a.speed) % 64] != 0) {
                        if (x >= 0) {
                            a.posy += a.speed;
                        }
                        else {
                            a.posy -= a.speed;
                        }
                    }
                    a.posy += a.speed;
                }
                else {
                    if (worldMap[a.posx % 64][(a.posy - a.speed) % 64] != 0) {
                        if (x >= 0) {
                            a.posx += a.speed;
                        }
                        else {
                            a.posx -= a.speed;
                        }
                    }
                    a.posy -= a.speed;
                }
            }
        }
    }
}

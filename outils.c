//
// Created by kyli8 on 12/06/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "struct.h"
#include "outils.h"
#include "gfxlib/include/GfxLib.h"


double Rdistance(double x1, double y1, double x2, double y2){
    return sqrt( (x2 -x1) * (x2 -x1) + (y2 -y1) * (y2 -y1) );  //formule de distance entre deux point en rond
}

void gereEnnemie(Ennemie *tab, int number){
    for (int i = 0; i < number; ++i) {
        iaEnnemie(tab[i]);
    }
}

/*void iaEnnemie(Ennemie a, double x, double y) {
    double dist = Rdistance(a.posx, a.posy, x, y);
    printf("%f", dist);
    if (dist <= a.rangeView) {
        if (dist <= a.rangeAttack) {
            player.defense(a);
        } else {
            double x = player.posx - a.posx;
            double y = player.posy - a.posy;
            if (fabs(x) > fabs(y)) {
                if (x >= 0) {
                    a.posx += a.speed;
                } else {
                    a.posx -= a.speed;
                }
            } else {
                if (y >= 0) {
                    a.posy += a.speed;
                } else {
                    a.posy -= a.speed;
                }
            }
        }
    }
}
*/
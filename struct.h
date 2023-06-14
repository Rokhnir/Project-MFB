//
// Created by kyli8 on 12/06/2023.
//

#ifndef WORKSPACE_STRUCT_H
#define WORKSPACE_STRUCT_H

typedef struct{
    double life;
    double posx;
    double posy;
    double dammage;
    double speed;
    double rangeView;
    double rangeAttack;
}Ennemie;

typedef struct{
    double posx;
    double posy;
    int ammo;
    double life;
    double dammage;
    void (*attack)(Ennemie*); //Pointer de fonction permet d'utiliser ses fonctions comme des méthodes approche C++
    void (*defense)();
}Player;

static Player player;
#endif //WORKSPACE_STRUCT_H

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
    unsigned long long lastMove;
}Ennemie;

typedef struct {
    char *name;
    int inLoader;
    int loader;
    int dammage;
    int fireRate;
    unsigned long long lastShoot;
}Weapon;

typedef struct{
    double posx;
    double posy;
    int ammo;
    double life;
    void (*attack)(Ennemie*); //Pointer de fonction permet d'utiliser ses fonctions comme des méthodes approche C++
    int (*defense)(Ennemie);
    Weapon equipped;
}Player;

static Player player;
#endif //WORKSPACE_STRUCT_H

//
// Created by kyli8 on 14/06/2023.
//
#include <string.h>
#include "struct.h"
#include <time.h>


void changeWeapon(Weapon a){
    player.equipped = a;
    player.equipped.lastShoot = time(NULL) + 2;
}

void reload(){
    if(player.equipped.inLoader == player.equipped.loader){
        return;
    }
    if (player.equipped.inLoader + player.ammo > player.equipped.loader){
        player.equipped.inLoader = player.equipped.loader;
    }
    else{
        player.equipped.inLoader += player.ammo;
    }
    player.equipped.lastShoot = time(NULL) + 2;
}

void fire(Ennemie *a){
    int t_time = time(NULL);
    if ( t_time < player.equipped.lastShoot + player.equipped.fireRate){
        return;
    }
    else if (player.ammo == 0){
        return;
    }
    else if(player.equipped.inLoader == 0){
        reload();
    }
    else{
        player.equipped.lastShoot = t_time;
        player.attack(a);
        --player.ammo;
    }
}
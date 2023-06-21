//
// Created by kyli8 on 14/06/2023.
//
#include <string.h>
#include <stdio.h>
#include "struct.h"
#include <time.h>
#include "outils.h"
#include "weapon.h"


void changeWeapon(Weapon a, Player *player){
    player->equipped = a;
    player->equipped.lastShoot = current_time_ms() + 2000;
    reload(player);
}

void reload(Player *player){
    /*if(player->equipped->inLoader == player->equipped->loader){
        printf("quit");
        return;
    }*/
    if (player->equipped.inLoader + player->ammo >= player->equipped.loader){
        player->equipped.inLoader = player->equipped.loader;
    }
    else{
        player->equipped.inLoader += player->ammo;
    }
    player->equipped.lastShoot = current_time_ms() + 2000;
}

void fire(Ennemie *a){
    int t_time = current_time_ms();
    if ( t_time < player.equipped.lastShoot + player.equipped.fireRate){
        return;
    }
    else if (player.ammo == 0){
        return;
    }
    else if(player.equipped.inLoader == 0){
        reload(&player);
    }
    else{
        player.equipped.lastShoot = t_time;
        player.attack(a);
        --player.ammo;
    }
}

float dda(float rayA){ //dda(30.) devrait tirer un rayon au mid

    int depth = 0, side = 0;
    float xOffset, yOffset, rayTan, distX = 100000.f, distY = 100000.f;

    //--- Vertical Check ---
    rayTan = tan(toRads(rayA));

    if(cos(toRads(rayA)) > 0.001){
        rayX = (((int)p.posX >> 6) << 6) + 64;
        rayY = (p.posX - rayX) * rayTan + p.posY;
        xOffset = 64;
        yOffset = -xOffset * rayTan;
    }
    else if(cos(toRads(rayA)) < 0.001){
        rayX = (((int)p.posX >> 6) << 6) - 0.0001;
        rayY = (p.posX - rayX) * rayTan + p.posY;
        xOffset = -64;
        yOffset = -xOffset * rayTan;
    }
    else {
        rayX += xOffset;
        rayY += yOffset;
        depth = mapHeight;
    }
    while(depth < mapHeight){
        mapX = (int)rayX >> 6;
        mapY = (int)rayY >> 6;
        if(map[mapX][mapY]){
            depth = mapHeight;
            distX = cos(toRads(rayA)) * (rayX - p.posX) - sin(toRads(rayA)) * (rayY - posY);
        }
        else {
            rayX += xOffset;
            rayY += yOffset;
            depth++;
        }
    }
    float verticalX = rayX;
    float verticalY = rayY;

    //--- Vertical Check ---
    depth = 0;
    rayTan = 1./rayTan;

    if(cos(toRads(rayA)) > 0.001){
        rayX = (p.posY - rayY) * rayTan + p.posX;
        rayY = (((int)p.posX >> 6) << 6) - 0.0001;
        xOffset = -64;
        yOffset = -yOffset * rayTan;
    }
    else if(cos(toRads(rayA)) < 0.001){
        rayX = (p.posY - rayY) * rayTan + p.posX;
        rayY = (((int)p.posX >> 6) << 6) + 64;
        xOffset = 64;
        yOffset = -yOffset * rayTan;
    }
    else {
        rayX += xOffset;
        rayY += yOffset;
        depth = mapWidth;
    }
    while(depth < mapWidth){
        mapX = (int)rayX >> 6;
        mapY = (int)rayY >> 6;
        if(map[mapX][mapY] != 0){
            depth = mapHeight;
            distY = cos(toRads(rayA)) * (rayX - p.posX) - sin(toRads(rayA)) * (rayY - posY);
        }
        else {
            rayX += xOffset;
            rayY += yOffset;
            depth++;
        }
    }

    //--- End ---
    float distance = fminf(distX, distY);
    int side = (distance == distX) ? -1 : 1;

    return distance * side * cos(toRads(fixAngle(p.dirA - rayA)));
}



int* dda2() { //dda(30.) devrait tirer un rayon au mid

    float rayA = 30;
    int depth = 0, side = 0;
    float xOffset, yOffset, rayTan, distX = 100000.f, distY = 100000.f;

    //--- Vertical Check ---
    rayTan = tan(toRads(rayA));

    if (cos(toRads(rayA)) > 0.001) {
        rayX = (((int) p.posX >> 6) << 6) + 64;
        rayY = (p.posX - rayX) * rayTan + p.posY;
        xOffset = 64;
        yOffset = -xOffset * rayTan;
    } else if (cos(toRads(rayA)) < 0.001) {
        rayX = (((int) p.posX >> 6) << 6) - 0.0001;
        rayY = (p.posX - rayX) * rayTan + p.posY;
        xOffset = -64;
        yOffset = -xOffset * rayTan;
    } else {
        rayX += xOffset;
        rayY += yOffset;
        depth = mapHeight;
    }
    while (depth < mapHeight) {
        mapX = (int) rayX >> 6;
        mapY = (int) rayY >> 6;
        if (map[mapX][mapY] > 0) {
            depth = mapHeight;
            distX = cos(toRads(rayA)) * (rayX - p.posX) - sin(toRads(rayA)) * (rayY - posY);
        }

        else if ( map[mapX][mapY] == -1){
            int coor[2];
            coor[0] = mapX;
            coor[1] = mapY;
            return coor;
        }

        else {
            rayX += xOffset;
            rayY += yOffset;
            depth++;
        }
    }
    float verticalX = rayX;
    float verticalY = rayY;

    //--- Vertical Check ---
    depth = 0;
    rayTan = 1. / rayTan;

    if (cos(toRads(rayA)) > 0.001) {
        rayX = (p.posY - rayY) * rayTan + p.posX;
        rayY = (((int) p.posX >> 6) << 6) - 0.0001;
        xOffset = -64;
        yOffset = -yOffset * rayTan;
    } else if (cos(toRads(rayA)) < 0.001) {
        rayX = (p.posY - rayY) * rayTan + p.posX;
        rayY = (((int) p.posX >> 6) << 6) + 64;
        xOffset = 64;
        yOffset = -yOffset * rayTan;
    } else {
        rayX += xOffset;
        rayY += yOffset;
        depth = mapWidth;
    }
    while (depth < mapWidth) {
        mapX = (int) rayX >> 6;
        mapY = (int) rayY >> 6;
        if (map[mapX][mapY] > 0) {
            depth = mapHeight;
            distY = cos(toRads(rayA)) * (rayX - p.posX) - sin(toRads(rayA)) * (rayY - posY);
        }
        else if ( map[mapX][mapY] == -1){
            int coor[2];
            coor[0] = mapX;
            coor[1] = mapY;
            return coor;
        }
        else {
            rayX += xOffset;
            rayY += yOffset;
            depth++;
        }
    }

    //--- End ---
}
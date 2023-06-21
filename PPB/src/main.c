#include <stdlib.h>
#include "../include/rayCasting.h"
#include "../include/mapHandler.h"
#include "../../gfxlib/include/GfxLib.h"
#include "../include/main.h"
#include <GL/glut.h>
#include <math.h>

int screenHeight = 640;
int screenWidth = 960;
extern int** map;
extern int mapWidth;
extern int mapHeight;
static KeysStruct Keys = {0,0,0,0};
Player p;
static float oldFrame = 0., newFrame = 0., fps = 0.;

int main(int argc, char **argv){

    createMap(0);
    p.posX = 150;
    p.posY = 400;
    p.dirA = 180;
    p.dirX = cos(toRads(p.dirA));
    p.dirY = -sin(toRads(p.dirA));

    initialiseGfx(argc, argv);
    prepareFenetreGraphique("Project-MBS", screenWidth, screenHeight);
    glutKeyboardUpFunc(keyUp);
    lanceBoucleEvenements();
    return 0;

}

void gestionEvenement(EvenementGfx evenement){
    switch(evenement){
        case Initialisation:
            //activeGestionDeplacementPassifSouris();
            demandeTemporisation(20);
            //freeMapMemory();
            break;
        case Temporisation:
            rafraichisFenetre();
            break;
        case Affichage:

            newFrame = glutGet(GLUT_ELAPSED_TIME);
            fps = newFrame - oldFrame;
            oldFrame = glutGet(GLUT_ELAPSED_TIME);

            effaceFenetre(0, 0, 0);

            if(Keys.q){
                p.dirA += 0.2 * fps;
                p.dirA = fixAngle(p.dirA);
                p.dirX = cos(toRads(p.dirA));
                p.dirY = -sin(toRads(p.dirA));
            }
            else if(Keys.d){
                p.dirA -= 0.2 * fps;
                p.dirA = fixAngle(p.dirA);
                p.dirX = cos(toRads(p.dirA));
                p.dirY = -sin(toRads(p.dirA));
            }

            if(Keys.z){
                float newPosX = p.posX + p.dirX * 0.2 * fps;
                float newPosY = p.posY + p.dirY * 0.2 * fps;
                if(!map[(int){newPosX} >> 6][(int){p.posY} >> 6]) p.posX = newPosX;
                if(!map[(int){p.posX} >> 6][(int){newPosY} >> 6]) p.posY = newPosY;
            }
            else if(Keys.s){
                float newPosX = p.posX - p.dirX * 0.2 * fps;
                float newPosY = p.posY - p.dirY * 0.2 * fps;
                if(!map[(int){newPosX} / 64][(int){p.posY} / 64]) p.posX = newPosX;
                if(!map[(int){p.posX} / 64][(int){newPosY} / 64]) p.posY = newPosY;
            }

            rayCasting();
            break;
        case Clavier: 
            keyDown(caractereClavier());
            break;
        case ClavierSpecial: break;
        case BoutonSouris: break;
        case Souris: break;
        case Inactivite: break;
        case Redimensionnement:
            screenWidth = largeurFenetre();
            screenHeight = hauteurFenetre();
            break;
    }
}

void keyUp(unsigned char key, int x, int y){
    switch(key){
        case 'Z':
        case 'z':
            Keys.z = 0;
            break;
        case 'Q':
        case 'q':
            Keys.q = 0;
            break;
        case 'S':
        case 's':
            Keys.s = 0;
            break;
        case 'D':
        case 'd':
            Keys.d = 0;
            break;
    }
    return;
}

void keyDown(unsigned char key){
    switch(key){
        case 'Z':
        case 'z':
            Keys.z = 1;
            break;
        case 'Q':
        case 'q':
            Keys.q = 1;
            break;
        case 'S':
        case 's':
            Keys.s = 1;
            break;
        case 'D':
        case 'd':
            Keys.d = 1;
            break;
    }
    return;
}

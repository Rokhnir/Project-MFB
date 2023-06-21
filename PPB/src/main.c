#include <stdlib.h>
#include "../include/rayCasting.h"
#include "../include/mapHandler.h"
#include "../../gfxlib/include/GfxLib.h"
#include "../include/main.h"
#include <GL/glut.h>
#include <math.h>

int screenHeight = 640;
int screenWidth = 960;
static KeysStruct Keys = {0,0,0,0};
Player p;
static float oldFrame = 0., newFrame = 0., fps = 0.;

int main(int argc, char **argv){

    createMap(0);
    p.posX = 400;
    p.posY = 150;
    p.dirA = 90;
    p.dirX = -sin(toRads(p.dirA));
    p.dirY = cos(toRads(p.dirA));

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
                p.dirX = -sin(toRads(p.dirA));
                p.dirY = cos(toRads(p.dirA));
            }
            else if(Keys.d){
                p.dirA -= 0.2 * fps;
                p.dirA = fixAngle(p.dirA);
                p.dirX = -sin(toRads(p.dirA));
                p.dirY = cos(toRads(p.dirA));
            }
            int xOffset;
            if(p.dirX < 0) xOffset = -20;
            else xOffset = 20;
            int yOffset;
            if(p.dirY < 0) yOffset = -20;
            else yOffset = 20;



            if(Keys.z){
                p.posX += p.dirX * 0.2 * fps;
                p.posY += p.dirY * 0.2 * fps;
            }
            else if(Keys.s){
                p.posX -= p.dirX * 0.2 * fps;
                p.posY -= p.dirY * 0.2 * fps;
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

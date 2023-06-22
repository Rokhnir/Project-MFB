//
// Created by kylm on 22/06/23.
//
//
// Created by kylm on 22/06/23.
//
#include <stdlib.h>
#include <stdio.h>
#include "../include/main.h" // self
#include "../../gfxlib/include/GfxLib.h" // *el famoso*
#include <GL/glut.h> // glutKeyboardUpFunc | glutget | GLUT_ELAPSED_TIME
#include "../include/struct.h" // rayCasting
#include "../../gfxlib/include/BmpLib.h" // Pour l'utilisation des bmp
#include "../../gfxlib/include/ESLib.h" // / Pour utiliser valeurAleatoire()
#include "../include/opacite.h" // / Pour utiliser valeurAleatoire()
#include "../include/ui.h" // / Pour utiliser valeurAleatoire()
#include <string.h>
#include "../include/playerHandler.h" // / Pour utiliser valeurAleatoire()

/*
DonneesImageRGB *image = NULL;
int *pixels = NULL;
Texture2D *texture = NULL;
int idArme = 0;



void ui() {
    if (strcmp(player.equipped.name, "Gun") == 0) {

        image = lisBMPRGB("image/pistol_front.bmp");
        pixels = BVR2ARVB1(image->largeurImage, image->hauteurImage, image->donneesRGB);
        texture = creeTexture2D(image->largeurImage, image->hauteurImage, pixels);
        rectangleSelonTexture((largeurFenetre() / 2) - 32, hauteurFenetre() / 8, texture);

    }
    if (strcmp(player.equipped.name, "Dupstep Gun") == 0) {


        image = lisBMPRGB("image/dubstep_gun_front.bmp");
        pixels = BVR2ARVB1(image->largeurImage, image->hauteurImage, image->donneesRGB);
        texture = creeTexture2D(image->largeurImage, image->hauteurImage, pixels);
        rectangleSelonTexture((largeurFenetre() / 2) - 32, hauteurFenetre() / 8, texture);

    }

    if (strcmp(player.equipped.name, "Rifle") == 0) {
        image = lisBMPRGB("image/fusil_front.bmp");
        pixels = BVR2ARVB1(image->largeurImage, image->hauteurImage, image->donneesRGB);
        texture = creeTexture2D(image->largeurImage, image->hauteurImage, pixels);
        rectangleSelonTexture((largeurFenetre() / 2) - 32, hauteurFenetre() / 8, texture);


    }


    newHUD();

}
void newHUD() {
    char buffer[7] = {0};
    couleurCourante(100, 100, 100);
    rectangle(0, 0, largeurFenetre(), hauteurFenetre() * 0.07);
    couleurCourante(0, 0, 128);

    rectangle(2, 0, largeurFenetre() / 9 - 2, hauteurFenetre() * 0.07);

    rectangle(largeurFenetre() / 9 + 2, 0, largeurFenetre() / 3 - 2, hauteurFenetre() * 0.07);

    rectangle(largeurFenetre() / 3 + 2, 0, 4 * largeurFenetre() / 9 - 2, hauteurFenetre() * 0.07);

    rectangle(4 * largeurFenetre() / 9 + 2, 0, 5 * largeurFenetre() / 9 - 2, hauteurFenetre() * 0.07);

    rectangle(5 * largeurFenetre() / 9 + 2, 0, 6 * largeurFenetre() / 9 - 2, hauteurFenetre() * 0.07);

    rectangle(6 * largeurFenetre() / 9 + 2, 0, 7 * largeurFenetre() / 9 - 2, hauteurFenetre() * 0.07);

    rectangle(7 * largeurFenetre() / 9 + 2, 0, largeurFenetre() - 2, hauteurFenetre() * 0.07);

    // ecriture :
    couleurCourante(255, 255, 255);

    afficheChaine("LEVEL :", 20, 2, hauteurFenetre() * 0.045);
    //sprintf(buffer, "%d", level);
    afficheChaine(buffer, 30, largeurFenetre() / 18, hauteurFenetre() * 0.02);

    afficheChaine("SCORE :", 20, largeurFenetre() / 9 + 4, hauteurFenetre() * 0.045);
    //sprintf(buffer, "%d", score);
    afficheChaine(buffer, 30, largeurFenetre() / 6, hauteurFenetre() * 0.02);

    afficheChaine("HEALTH :", 20, largeurFenetre() / 3 + 4, hauteurFenetre() * 0.045);
    //sprintf(buffer, "%d", (int) player.life);
    afficheChaine(buffer, 30, 7 * largeurFenetre() / 18, hauteurFenetre() * 0.02);

    afficheChaine("LOADER :", 20, 5 * largeurFenetre() / 9 + 4, hauteurFenetre() * 0.045);
   // sprintf(buffer, "%d", player.equipped.inLoader);
    afficheChaine(buffer, 30, 11 * largeurFenetre() / 18, hauteurFenetre() * 0.02);

    afficheChaine("AMMO :", 20, 6 * largeurFenetre() / 9 + 4, hauteurFenetre() * 0.045);
   // sprintf(buffer, "%d", player.ammo);
    afficheChaine(buffer, 30, 13 * largeurFenetre() / 18, hauteurFenetre() * 0.02);
}*/

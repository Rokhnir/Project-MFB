//
// Created by kylm on 22/06/23.
//


#include "../../gfxlib/include/BmpLib.h" // Pour l'utilisation des bmp
#include <stdio.h>
#include <stdlib.h>
#include "../../gfxlib/include/GfxLib.h" // *el famoso*


int *BVR2ARVB1(int largeur, int hauteur, const unsigned char *donnees) {
    // printf("BVR");

    const unsigned char *ptrDonnees;
    unsigned char *pixels = (unsigned char *) malloc(largeur * hauteur * 4 * sizeof(unsigned char));
    unsigned char *ptrPixel;
    int index;
    ptrPixel = pixels;
    ptrDonnees = donnees;
    for (index = largeur * hauteur; index != 0; --index) {

        /* On parcourt tous les pixels de l'image */
        if (ptrDonnees[0] == 0xFF && ptrDonnees[1] == 0x00 && ptrDonnees[2] == 0xFF) {

            ptrPixel[0] = ptrDonnees[0];
            ptrPixel[1] = ptrDonnees[1];
            ptrPixel[2] = ptrDonnees[2];
            ptrPixel[3] = 0x00; // Opacité à 0
        } else {
            ptrPixel[0] = ptrDonnees[2];
            ptrPixel[1] = ptrDonnees[1];
            ptrPixel[2] = ptrDonnees[0];
            ptrPixel[3] = 0xFF; // Opacité à 255
        }
        ptrDonnees += 3; /* On passe à la première composante du pixel suivant */
        ptrPixel += 4;     /* On passe au pixel suivant */
    }
    return (int *) pixels;
}
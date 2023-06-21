#include <stdlib.h>
#include "../include/rayCasting.h"
#include "../include/mapHandler.h"
#include "../../gfxlib/include/GfxLib.h"

int screenHeightt = 300;
int screenWidtht = 800;

int main(int argc, char **argv){

    initialiseGfx(argc, argv);
    prepareFenetreGraphique("Project-MBS", screenWidtht, screenHeightt);
    lanceBoucleEvenements();
    return 0;

}

void gestionEvenement(EvenementGfx evenement){
    switch(evenement){
        case Initialisation:
            activeGestionDeplacementPassifSouris();
            demandeTemporisation(20);
            createMap(1);
            //freeMapMemory();
            break;
        case Temporisation:
            rafraichisFenetre();
            break;
        case Affichage:
            effaceFenetre(0, 0, 0);
            rayCasting();
            break;
        case Clavier: break;
        case ClavierSpecial: break;
        case BoutonSouris: break;
        case Souris: break;
        case Inactivite: break;
        case Redimensionnement:
            screenWidtht = largeurFenetre();
            screenHeightt = hauteurFenetre();
            break;
    }
}
#include <stdlib.h>

int main(int argc, char **argv){

    initialiseGfx(argc, argv);
    prepareFenetreGraphique("Project-MBS", screenWidth, screenHeight);
    lanceBoucleEvenements();
    return 0;

}

void gestionEvenement(EvenementGfx evenement){
    switch(evenement){
        case Initialisation:
            activeGestionDeplacementPassifSouris();
            demandeTemporisation(20);
            createMap();
            break;h
        case Temporisation:
            rafraichisFenetre();
            break;
        case Affichage:
            effaceFenetre(0, 0, 0);
            raycaster();
            break;
        case Clavier: break;
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
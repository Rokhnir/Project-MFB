/* ----------------------------------------------------------- */
// INCLUDES
//
#include "../include/main.h" // self
#include "../include/mapHandler.h" // createMap
#include "../../gfxlib/include/GfxLib.h" // *el famoso*
#include <GL/glut.h> // glutKeyboardUpFunc | glutget | GLUT_ELAPSED_TIME
#include "../include/playerHandler.h" // movePlayer
#include "../include/rayCasting.h" // rayCasting
#include "../../gfxlib/include/BmpLib.h" // DonneesImageRGB | lisBMPRGB | ecrisImage
#include <stdio.h> // NULL

/* ----------------------------------------------------------- */
// GLOBAL VARS
//
int screenHeight = 640; // Hauteur de la fenêtre (AR 3:2)
int screenWidth = 960; // Largeur de la fenêtre (AR 3:2)
float fps = 0.; // Permet de garder une vitesse de déplacement constante
//
static KeysStruct Keys = {0,0,0,0}; // Structure conservant l'état des touches
static float oldFrame = 0., newFrame = 0.; // Permet le calcul des fps
static int gameState = 0; // Stocke l'état actuel du jeu

/* ----------------------------------------------------------- */
// FUNCTIONS
//
int main(int argc, char **argv){

    createMap(1);

    initialiseGfx(argc, argv);
    
    prepareFenetreGraphique("Project-MBS", screenWidth, screenHeight);
    
    glutKeyboardUpFunc(keyUp); // Event glut détectant le relâchement d'une touche

    lanceBoucleEvenements();
    
    return 0;

}

void gestionEvenement(EvenementGfx evenement){
    switch(evenement){
        case Initialisation:
            demandeTemporisation(20);
            gameStart();
            static DonneesImageRGB *wallTexture = NULL;
                wallTexture = lisBMPRGB("./assets/textures/wall.bmp");
            static DonneesImageRGB *enemyTexture = NULL;
                enemyTexture = lisBMPRGB("./assets/textures/enemy.bmp");
            break;
        case Temporisation:
            rafraichisFenetre();
            break;
        case Affichage:

            switch(gameState){
                case 0:
                    gameStart();
                    break;
                default:
                    newFrame = glutGet(GLUT_ELAPSED_TIME);
                    fps = newFrame - oldFrame;
                    oldFrame = glutGet(GLUT_ELAPSED_TIME);

                    effaceFenetre(0, 0, 0);

                    if(Keys.z) movePlayer('z');
                    if(Keys.q) movePlayer('q');
                    if(Keys.s) movePlayer('s');
                    if(Keys.d) movePlayer('d');
                    
                    rayCasting(wallTexture);

                    ///posX = j * 64. + 32.;
                    //posY = i * 64. + 32.;
                    //drawEnemy(posX, posY, enemyTexture);
                    epaisseurDeTrait(5.);
                    couleurCourante(255, 255, 255);
                    ligne((screenWidth-960)/2 - 5., (screenHeight-640)/2 - 5., (screenWidth-960)/2 - 5., (screenHeight-640)/2 + 645);
                    ligne((screenWidth-960)/2 - 5., (screenHeight-640)/2 + 645, (screenWidth-960)/2 + 965., (screenHeight-640)/2 + 645);
                    ligne((screenWidth-960)/2 + 965., (screenHeight-640)/2 + 645, (screenWidth-960)/2 + 965., (screenHeight-640)/2 - 5.);
                    ligne((screenWidth-960)/2 + 965., (screenHeight-640)/2 - 5., (screenWidth-960)/2 - 5., (screenHeight-640)/2 - 5.);
                    
                    break;
            }

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
        case 'X':
        case 'x':
            termineBoucleEvenements();
            freeMapMemory();
            break;
        case 13:
            if(gameState == 0){
                gameState = 1;
            };
            break;
    }
    return;
}

void gameStart(void){

    modePleinEcran();
    DonneesImageRGB *texture = NULL;
    texture = lisBMPRGB("./assets/textures/startScreen.bmp");
    if(texture != NULL) ecrisImage(0, 0, texture->largeurImage, texture->hauteurImage, texture->donneesRGB);

}
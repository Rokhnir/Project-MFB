/* ----------------------------------------------------------- */
// INCLUDES
//
#include "../include/main.h" // self
#include "../include/mapHandler.h" // createMap
#include "../../gfxlib/include/GfxLib.h" // *el famoso*
#include <GL/glut.h> // glutKeyboardUpFunc | glutget | GLUT_ELAPSED_TIME
#include "../include/playerHandler.h" // movePlayer
#include "../include/rayCasting.h" // rayCasting
#include "../include/ui.h"
#include "../include/outils.h"
#include "../include/opacite.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "pthread.h"
#include <stdbool.h>

//Gestion musique + hud


//Images


/* ----------------------------------------------------------- */
// GLOBAL VARS
//
int screenHeight = 640; // Hauteur de la fenêtre
int screenWidth = 960; // Largeur de la fenêtre
float fps = 0.; // Permet de garder une vitesse de déplacement constante
//
static KeysStruct Keys = {0,0,0,0}; // Structure conservant l'état des touches
static float oldFrame = 0., newFrame = 0.; // Permet le calcul des fps
static char Music[] = {"assets/music/DoomOSTBetter.wav"};
static bool is_hud_draw =false;
/* ----------------------------------------------------------- */
// FUNCTIONS
//

DonneesImageRGB *image = NULL;
int *pixels = NULL;
Texture2D *texture = NULL;
int idArme = 0;

void arret_brutal_music(void) {
    char command[256];
    sprintf(command, "killall aplay");
    system(command);
}
void play_sound(const char *filename) {
    loopThreadSond = true;
    char command[256];
    sprintf(command, "aplay %s ", filename);
    system(command);
    loopThreadSond = false;
    pthread_exit(NULL);
}
void play_music(void *arg) {
    char command[256];
    sprintf(command, "aplay %s", Music);
    system(command);
    loopThreadStarted2 = false;

}
void *play_sound_parallel(void *filename) {
    play_sound((const char *) filename);

    pthread_exit(NULL);
}

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
    if (is_hud_draw == false){
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
  //  afficheChaine(buffer, 30, largeurFenetre() / 18, hauteurFenetre() * 0.02);

    afficheChaine("SCORE :", 20, largeurFenetre() / 9 + 4, hauteurFenetre() * 0.045);
    //sprintf(buffer, "%d", score);
  //  afficheChaine(buffer, 30, largeurFenetre() / 6, hauteurFenetre() * 0.02);

    afficheChaine("HEALTH :", 20, largeurFenetre() / 3 + 4, hauteurFenetre() * 0.045);
    sprintf(buffer, "%d", (int) player.life);
    afficheChaine(buffer, 30, 7 * largeurFenetre() / 18, hauteurFenetre() * 0.02);

    afficheChaine("LOADER :", 20, 5 * largeurFenetre() / 9 + 4, hauteurFenetre() * 0.045);
    // sprintf(buffer, "%d", player.equipped.inLoader);
  //  afficheChaine(buffer, 30, 11 * largeurFenetre() / 18, hauteurFenetre() * 0.02);

    afficheChaine("AMMO :", 20, 6 * largeurFenetre() / 9 + 4, hauteurFenetre() * 0.045);
    // sprintf(buffer, "%d", player.ammo);
   // afficheChaine(buffer, 30, 13 * largeurFenetre() / 18, hauteurFenetre() * 0.02);
    //is_hud_draw = true;
    }
}

void *loopThread(void *arg) {
    lanceBoucleEvenements();

    return NULL;
}
int main(int argc, char **argv) {
    if (loopThreadStarted == false) {

    createMap(1);

    initialiseGfx(argc, argv);

    prepareFenetreGraphique("Project-MBS", screenWidth, screenHeight);

    glutKeyboardUpFunc(keyUp); // Event glut détectant le relâchement d'une touche
}
//###################MUSIC###################

    pthread_t thread;
    if (loopThreadStarted == false) {
        pthread_create(&thread, NULL, loopThread, NULL);
        loopThreadStarted = true;
    }
    pthread_t thread2;
    if (loopThreadStarted2 == false) {

        pthread_create(&thread2, NULL, play_music, NULL);
        loopThreadStarted2 = true;
    }
    if (arret == true) { arret_brutal_music(); }

    pthread_join(thread2, NULL);

    return main(argc, argv);

// ###########################################

}

void gestionEvenement(EvenementGfx evenement){
    switch(evenement){
        case Initialisation:

            demandeTemporisation(20);
            break;
        case Temporisation:
            rafraichisFenetre();
            break;
        case Affichage:

            newFrame = glutGet(GLUT_ELAPSED_TIME);
            fps = newFrame - oldFrame;
            oldFrame = glutGet(GLUT_ELAPSED_TIME);

            effaceFenetre(0, 0, 0);

            if(Keys.z) movePlayer('z');
            if(Keys.q) movePlayer('q');
            if(Keys.s) movePlayer('s');
            if(Keys.d) movePlayer('d');


            rayCasting();
                //printf("AAAAAAA");
                newHUD();


            break;
        case Clavier:
            keyDown(caractereClavier());
            break;
        case 'X':
        case 'x':
            arret = true;
            arret_brutal_music();
            termineBoucleEvenements();
            //send_letter_to_buffer(caractereClavier());

            //strcpy(FILENAME, "sus.wav");
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
            arret_brutal_music();
            arret = true;
            termineBoucleEvenements();
            freeMapMemory();
            break;
    }
    return;
}
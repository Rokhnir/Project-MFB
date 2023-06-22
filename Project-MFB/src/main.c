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
#include "../include/struct.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "pthread.h"
#include <stdbool.h>
#include <time.h>
#include <math.h>

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
bool loopThreadStarted = false;
bool loopThreadStarted2 = false;
bool loopThreadSond = false;

static int gameState = 0;

static int score= 0;
static int level = 1;
static int nrbEnnemie = 3;
static Ennemie lstEnnemie[3];


bool arret = false;
char cheatCode[10] = {};
/* ----------------------------------------------------------- */
// FUNCTIONS
//

DonneesImageRGB *image = NULL;
int *pixels = NULL;
Texture2D *texture = NULL;
int idArme = 0;


int main(int argc, char **argv) {

    lstEnnemie[0] = initEnnemie(2,2);
    lstEnnemie[1] = initEnnemie(14,4);
    lstEnnemie[2] = initEnnemie(22,10);

    //player.posx = 11;
    //player.posy = 12;
    player.ammo = 30;
    player.life = 100;
    player.attack = attacks;
    player.defense = defenses;

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

void attacks(Ennemie *a){
    a->life -= player.equipped.dammage;
}

int defenses(Ennemie a){
    srandom(time(NULL));
    int x = random() % 101;
    if (x <= 50){
        player.life = fmax(player.life - a.dammage, 0);
    }
    return 0;
}

Ennemie initEnnemie(int x, int y){
    Ennemie a;
    a.life = 10;
    a.posx = x;
    a.posy = y;
    a.dammage = 10;
    a.speed = 1;
    a.rangeView = 3;
    a.rangeAttack = 2;
    a.lastMove = current_time_ms();
    return a;
}

void iaEnnemie(Ennemie a) {
    if(a.lastMove + 33 < current_time_ms()){
        return;
    }
    a.lastMove = current_time_ms();
    double dist = Rdistance((double)a.posx, (double)a.posy, (player.posx / 64), (player.posy / 64));
    if (dist <= a.rangeView) {
        if (dist <= a.rangeAttack) {
            player.defense(a);
        } else {
            map[a.posx][a.posy] = 0;
            double x = player.posx - a.posx;
            double y = player.posy - a.posy;
            if (fabs(x) > fabs(y)) {
                if (x >= 0) {
                    if ( map[(int)(a.posx + a.speed)][a.posy] > 0 ){
                        if (y >= 0) {
                            a.posy += a.speed;
                            map[a.posx][a.posy] = -1;
                        } else {
                            a.posy -= a.speed;
                            map[a.posx][a.posy] = -1;
                        }
                    }
                    a.posx += a.speed;
                    map[a.posx][a.posy] = -1;
                } else {
                    if ( map[(int)(a.posx - a.speed)][a.posy] > 0 ){
                        if (y >= 0) {
                            a.posy += a.speed;
                            map[a.posx][a.posy] = -1;
                        } else {
                            a.posy -= a.speed;
                            map[a.posx][a.posy] = -1;
                        }
                    }
                    a.posx -= a.speed;
                    map[a.posx][a.posy] = -1;
                }
            } else {
                if (y >= 0) {
                    if ( map[a.posx][(int)(a.posy + a.speed)] > 0 ){
                        if (y >= 0) {
                            a.posx += a.speed;
                            map[a.posx][a.posy] = -1;
                        } else {
                            a.posx -= a.speed;
                            map[a.posx][a.posy] = -1;
                        }
                    }
                    a.posy += a.speed;
                    map[a.posx][a.posy] = -1;
                } else {
                    if ( map[a.posx][ (int)(a.posy - a.speed)] > 0 ){
                        if (y >= 0) {
                            a.posx += a.speed;
                            map[a.posx][a.posy] = -1;
                        } else {
                            a.posx -= a.speed;
                            map[a.posx][a.posy] = -1;
                        }
                    }
                    a.posy -= a.speed;
                    map[a.posx][a.posy] = -1;
                }
            }
        }
    }
}

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
        rectangleSelonTexture((screenWidth / 2) - 32, screenHeight / 8, texture);

    }
    if (strcmp(player.equipped.name, "Dupstep Gun") == 0) {


        image = lisBMPRGB("image/dubstep_gun_front.bmp");
        pixels = BVR2ARVB1(image->largeurImage, image->hauteurImage, image->donneesRGB);
        texture = creeTexture2D(image->largeurImage, image->hauteurImage, pixels);
        rectangleSelonTexture((screenWidth / 2) - 32, screenHeight / 8, texture);

    }

    if (strcmp(player.equipped.name, "Rifle") == 0) {
        image = lisBMPRGB("image/fusil_front.bmp");
        pixels = BVR2ARVB1(image->largeurImage, image->hauteurImage, image->donneesRGB);
        texture = creeTexture2D(image->largeurImage, image->hauteurImage, pixels);
        rectangleSelonTexture((screenWidth / 2) - 32, screenHeight / 8, texture);


    }


    newHUD();

}
void newHUD() {
    char buffer[7] = {0};
    couleurCourante(100, 100, 100);
    rectangle(0, 0, screenWidth, screenHeight * 0.07);
    couleurCourante(0, 0, 128);

    rectangle(2, 0, screenWidth / 9 - 2, screenHeight * 0.07);

    rectangle(screenWidth / 9 + 2, 0, screenWidth / 3 - 2, screenHeight * 0.07);

    rectangle(screenWidth / 3 + 2, 0, 4 * screenWidth / 9 - 2, screenHeight * 0.07);

    rectangle(4 * screenWidth / 9 + 2, 0, 5 * screenWidth / 9 - 2, screenHeight * 0.07);

    rectangle(5 * screenWidth / 9 + 2, 0, 6 * screenWidth / 9 - 2, screenHeight * 0.07);

    rectangle(6 * screenWidth / 9 + 2, 0, 7 * screenWidth / 9 - 2, screenHeight * 0.07);

    rectangle(7 * screenWidth / 9 + 2, 0, screenWidth - 2, screenHeight * 0.07);

    // ecriture :
    couleurCourante(255, 255, 255);
    epaisseurDeTrait(3);
    afficheChaine("LEVEL :", 20, 2, screenHeight * 0.045);
    sprintf(buffer, "%d", level);
    afficheChaine(buffer, 30, screenWidth / 18, screenHeight * 0.02);

    afficheChaine("SCORE :", 20, screenWidth / 9 + 4, screenHeight * 0.045);
    sprintf(buffer, "%d", score);
    afficheChaine(buffer, 30, screenWidth / 6, screenHeight * 0.02);

    afficheChaine("HEALTH :", 20, screenWidth / 3 + 4, screenHeight * 0.045);
    sprintf(buffer, "%d", (int) player.life);
    afficheChaine(buffer, 30, 7 * screenWidth / 18, screenHeight * 0.02);

    afficheChaine("LOADER :", 20, 5 * screenWidth / 9 + 4, screenHeight * 0.045);
    sprintf(buffer, "%d", player.equipped.inLoader);
    afficheChaine(buffer, 30, 11 * screenWidth / 18, screenHeight * 0.02);

    afficheChaine("AMMO :", 20, 6 * screenWidth / 9 + 4, screenHeight * 0.045);
    sprintf(buffer, "%d", player.ammo);
    afficheChaine(buffer, 30, 13 * screenWidth / 18, screenHeight * 0.02);
}

void *loopThread(void *arg) {
    lanceBoucleEvenements();

    return NULL;
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

                    for (int i = 0; i < mapWidth; ++i) {
                        for (int j = 0; j < mapHeight; ++j) {
                            if(map[i][j] == -1){
                                int posX = j * 64. + 32.;
                                int posY = i * 64. + 32.;
                                drawEnemy(posX, posY, enemyTexture);
                            }
                        }
                    }
                    epaisseurDeTrait(5.);
                    couleurCourante(255, 255, 255);
                    ligne((screenWidth-960)/2 - 5., (screenHeight-640)/2 - 5., (screenWidth-960)/2 - 5., (screenHeight-640)/2 + 645);
                    ligne((screenWidth-960)/2 - 5., (screenHeight-640)/2 + 645, (screenWidth-960)/2 + 965., (screenHeight-640)/2 + 645);
                    ligne((screenWidth-960)/2 + 965., (screenHeight-640)/2 + 645, (screenWidth-960)/2 + 965., (screenHeight-640)/2 - 5.);
                    ligne((screenWidth-960)/2 + 965., (screenHeight-640)/2 - 5., (screenWidth-960)/2 - 5., (screenHeight-640)/2 - 5.);

                    newHUD();
                    //ui();
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
            arret = true;
            arret_brutal_music();
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
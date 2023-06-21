#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "gfxlib/include/GfxLib.h"
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include "gfxlib/include/WavLib.h"
#include "gfxlib/include/BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "gfxlib/include/ESLib.h"    // Pour utiliser valeurAleatoire()
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "struct.h"
#include "test.h"
#include <string.h>
#include "outils.h"
#include "weapon.h"

Ennemie test;
static int score = 0;
static int level = 1;

static Weapon dupStep = {"Dupstep Gun", 0, 6, 10, 900, 0};
static Weapon rifle = {"Rifle", 0, 30, 5, 250, 0};
static Weapon gun = {"Gun", 0, 12, 7, 700, 0};

#define screenWidth 1920
#define screenHeight 1080
#define mapWidth 24
#define mapHeight 24

static char Music[] = {"music/DoomOSTBetter.wav"};
static bool loopThreadStarted = false;
static bool loopThreadStarted2 = false;
static bool loopThreadSond = false;

static bool arret = false;
static char cheatCode[10] = {};

double pX;
double pY;
double planeX = 0;
double planeY = screenWidth;
double dirX;
double dirY;
double pvecX;
double pvecY;
double dvecX;
double dvecY;
double angle;
double oldDirX;
double oldPlaneX;

//################ Gestion Images ###################
DonneesImageRGB *image = NULL;
int *pixels = NULL;
Texture2D *texture = NULL;
int idArme = 0;
//####################################################


static int *BVR2ARVB1(int largeur, int hauteur, const unsigned char *donnees) {
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

void gestionEvenement(EvenementGfx evenement);

void loop();

void ui();

int worldMap[mapWidth][mapHeight];

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

void send_letter_to_buffer(char letter) {
    int i = 0;
    while (cheatCode[i] != '\0') {
        i++;
    }
    cheatCode[i] = letter;
    cheatCode[i + 1] = '\0';
    if (strcmp(cheatCode, "uwu") == 0) {
        printf("Cheat code sus\n");
        const char *filename = "music/sus.wav";

        pthread_t thread;
        pthread_create(&thread, NULL, play_sound_parallel, (void *) filename);
        pthread_detach(thread);
        cheatCode[0] = '\0';
    }
    if (i > 10) {
        cheatCode[0] = '\0';
    }
    printf("Cheat code %s\n", cheatCode);

}

void *loopThread(void *arg) {
    lanceBoucleEvenements();

    return NULL;
}

void attacks(Ennemie *a) {
    a->life -= player.equipped.dammage;
}

int defenses(Ennemie a) {
    srandom(time(NULL));
    int x = random() % 101;
    if (x <= 50) {
        player.life = fmax(player.life - a.dammage, 0);
    }
    return 0;
}

void iaEnnemie(Ennemie a) {
    if (a.lastMove + 33 < current_time_ms()) {
        return;
    }
    a.lastMove = current_time_ms();
    double dist = Rdistance(a.posx, a.posy, player.posx, player.posy);
    if (dist <= a.rangeView) {
        if (dist <= a.rangeAttack) {
            player.defense(a);
        } else {
            double x = player.posx - a.posx;
            double y = player.posy - a.posy;
            if (fabs(x) > fabs(y)) {
                if (x >= 0) {
                    a.posx += a.speed;
                } else {
                    a.posx -= a.speed;
                }
            } else {
                if (y >= 0) {
                    a.posy += a.speed;
                } else {
                    a.posy -= a.speed;
                }
            }
        }
    }
}

/*
void afficheAmmo(){
    char buff[20] = {0}, buff1[3];
    sprintf(buff1,"%d",player.equipped.inLoader);
    strcat(buff,buff1);
    strcat(buff, " / ");
    sprintf(buff1,"%d", player.equipped.loader);
    strcat(buff, buff1);
    //afficheChaine(buff,largeurFenetre()*0.015 , 0.77 * largeurFenetre(), 50);
    strcat(buff," ");
    sprintf(buff1,"%d", player.ammo);
    strcat(buff, buff1);
    couleurCourante(255,0,0);
    epaisseurDeTrait(largeurFenetre()*0.003);
    //printf("%s  \n", buff);
    afficheChaine(buff,largeurFenetre()*0.015 , 0.77 * largeurFenetre(), 50);
}

void afficheHp(int debut, int longeur){
    couleurCourante(128,0,0);
    rectangle(debut,20,debut + longeur,40);
    couleurCourante(0,255,0);
    if (player.life <= 0){
        return;
    }
    rectangle(debut,20, (debut + longeur)/ 100 * (int)player.life , 40);
}
*/

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
    sprintf(buffer, "%d", level);
    afficheChaine(buffer, 30, largeurFenetre() / 18, hauteurFenetre() * 0.02);

    afficheChaine("SCORE :", 20, largeurFenetre() / 9 + 4, hauteurFenetre() * 0.045);
    sprintf(buffer, "%d", score);
    afficheChaine(buffer, 30, largeurFenetre() / 6, hauteurFenetre() * 0.02);

    afficheChaine("HEALTH :", 20, largeurFenetre() / 3 + 4, hauteurFenetre() * 0.045);
    sprintf(buffer, "%d", (int) player.life);
    afficheChaine(buffer, 30, 7 * largeurFenetre() / 18, hauteurFenetre() * 0.02);

    afficheChaine("LOADER :", 20, 5 * largeurFenetre() / 9 + 4, hauteurFenetre() * 0.045);
    sprintf(buffer, "%d", player.equipped.inLoader);
    afficheChaine(buffer, 30, 11 * largeurFenetre() / 18, hauteurFenetre() * 0.02);

    afficheChaine("AMMO :", 20, 6 * largeurFenetre() / 9 + 4, hauteurFenetre() * 0.045);
    sprintf(buffer, "%d", player.ammo);
    afficheChaine(buffer, 30, 13 * largeurFenetre() / 18, hauteurFenetre() * 0.02);
}

int main(int argc, char **argv) {

    if (loopThreadStarted == false) {
        player.posx = 11;
        player.posy = 12;
        player.ammo = 20;
        player.life = 100;
        player.attack = attacks;
        player.defense = defenses;

        test.life = 10;
        test.posx = 10;
        test.posy = 12;
        test.dammage = 10;
        test.speed = 5;
        test.rangeView = 17;
        test.rangeAttack = 17;
        test.lastMove = current_time_ms();

        gun.lastShoot = current_time_ms();
        rifle.lastShoot = current_time_ms();
        dupStep.lastShoot = current_time_ms();
        player.equipped = gun;
        /*--------------------MAP--------------------*/



        FILE *filemap = fopen("map", "r");

        if (filemap == NULL) {
            printf("Impossible d'ouvrir le fichier.\n");
            return 1;
        }

        for (int i = 0; i < mapHeight; i++) {
            for (int j = 0; j < mapWidth; j++) {
                fscanf(filemap, "%d", &worldMap[j][i]);
            }
        }

        fclose(filemap);


        for (int i = 0; i < mapHeight; i++) {
            for (int j = 0; j < mapWidth; j++) {
                printf("%d ", worldMap[j][i]);
            }
            printf("\n");
        }

        /*----------------------------------*/




        initialiseGfx(argc, argv);
        prepareFenetreGraphique("Doom", screenWidth, screenHeight);
        pX = hauteurFenetre() * 0.6;
        pY = hauteurFenetre() * 0.1;
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

void loop() {

    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 24; j++) {
            couleurCourante(50 * worldMap[i][j], 50 * worldMap[i][j], 50 * worldMap[i][j]);
            rectangle((hauteurFenetre() / 24) * (j), (hauteurFenetre() / 24) * (24 - i),
                      (hauteurFenetre() / 24) * (j + 1), (hauteurFenetre() / 24) * (23 - i));

        }

    }

    ui();

    epaisseurDeTrait(1.f);

    int length = planeY;

    for (int i = 0; i < screenWidth; i++) {
        double fovX = 2 * i / (double) screenWidth - 1;
        double rayDirX = dirX + planeX * fovX;
        double rayDirY = dirY + planeY * fovX;

        int steps = fmax(abs(rayDirX), abs(rayDirY));
        double incY = (!steps) ? 0 : rayDirX / steps;
        double incX = (!steps) ? 0 : rayDirY / steps;

        double xa = pX, ya = pY;
        int hit = 0;

        while (!hit) {
            xa += incX;
            ya += incY;
            int ouiX = floor(xa / (hauteurFenetre() / 24));
            int ouiY = floor(ya / (hauteurFenetre() / 24));
            if (worldMap[23 - ouiY][ouiX] > 0) hit = 1;
            if (hit) {
                double test = hauteurFenetre() / 24 * ouiX;
                double test2 = hauteurFenetre() / 24 * (ouiX + 1);
                int value;
                if (((floor(xa) < test + 1) && (floor(xa) > test - 1)) ||
                    ((floor(xa) < test2 + 1) && (floor(xa) > test2 - 1)))
                    value = 255;
                else value = 125;
                switch (worldMap[23 - ouiY][ouiX]) {
                    case 1:
                        couleurCourante(value, 0, 0);
                        break;
                    case 2:
                        couleurCourante(0, value, 0);
                        break;
                    case 3:
                        couleurCourante(0, 0, value);
                        break;
                    case 4:
                        couleurCourante(value, value, value);
                        break;
                    default:
                        couleurCourante(0, value, value);
                        break;
                }
            }
        }
        double ax = rayDirX - pX;
        double ay = rayDirY - pY;
        double bx = dirX - pX;
        double by = dirY - pY;
        double ab = ax * bx + ay * by;
        double aa = sqrt(pow(ax, 2) + pow(ay, 2));
        double bb = sqrt(pow(bx, 2) + pow(by, 2));
        double sino = (ab / (aa * bb));
        int dist = floor(sqrt(pow(xa - pX, 2) + pow(ya - pY, 2)));
        int lineHeight = screenHeight / dist * 20;

        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if (drawStart < 0)drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if (drawEnd >= screenHeight)drawEnd = screenHeight - 1;
        ligne(i, drawStart, i, drawEnd);
    }
}

void gestionEvenement(EvenementGfx evenement) {

    static bool pleinEcran = false;
    static DonneesImageRGB *image = NULL; // L'image a afficher au centre de l'ecran

    switch (evenement) {

        case Initialisation:

            activeGestionDeplacementPassifSouris();
            demandeTemporisation(20);
            dirX = (planeY / 0.66);
            dirY = 0;
            pvecX = pX;
            pvecY = 0;
            dvecX = abscisseSouris() - pX;
            dvecY = ordonneeSouris() - pY;
            angle = acos(pvecX * dvecX / (sqrt(pvecX * pvecX) * sqrt(dvecX * dvecX + dvecY * dvecY))) * 180 /
                    3.14159265;
            break;

        case Temporisation:
            rafraichisFenetre();
            break;

        case Affichage:
            effaceFenetre(0, 0, 0);

            loop();
            break;

        case Clavier:
            switch (caractereClavier()) {
                case 'R':
                case 'r':
                    reload(&player);
                    break;
                case '&':
                case '1':
                    changeWeapon(gun, &player);
                    break;
                    //case 'é':
                case '2':
                    changeWeapon(rifle, &player);
                    break;
                case '"':
                case '3':
                    changeWeapon(dupStep, &player);
                    break;


                case 'X':
                case 'x':
                    arret = true;
                    arret_brutal_music();
                    termineBoucleEvenements();
                    //send_letter_to_buffer(caractereClavier());

                    //strcpy(FILENAME, "sus.wav");
                    break;
                case 'F':
                case 'f':
                    pleinEcran = !pleinEcran;
                    if (pleinEcran)
                        modePleinEcran();
                    else
                        redimensionneFenetre(screenWidth, screenHeight);
                    //send_letter_to_buffer(caractereClavier());

                    break;
                case 'Z':
                case 'z':

                    pY += hauteurFenetre() * 0.05;
                    if (loopThreadSond == false) {
                        const char *filename = "music/pas.wav";

                        pthread_t thread;
                        pthread_create(&thread, NULL, play_sound_parallel, (void *) filename);
                        pthread_detach(thread);
                    }
                    //send_letter_to_buffer(caractereClavier());

                    break;
                case 'S':
                case 's':
                    pY -= hauteurFenetre() * 0.05;
                    if (loopThreadSond == false) {
                        const char *filename = "music/pas.wav";

                        pthread_t thread;
                        pthread_create(&thread, NULL, play_sound_parallel, (void *) filename);
                        pthread_detach(thread);
                    }
                    //send_letter_to_buffer(caractereClavier());

                    break;

                case 'Q':
                case 'q':
                    oldDirX = dirX;
                    dirX = dirX * cos(-0.1) - dirY * sin(-0.1);
                    dirY = oldDirX * sin(-0.1) + dirY * cos(-0.1);
                    oldPlaneX = planeX;
                    planeX = planeX * cos(-0.1) - planeY * sin(-0.1);
                    planeY = oldPlaneX * sin(-0.1) + planeY * cos(-0.1);
                    //send_letter_to_buffer(caractereClavier());

                    break;
                case 'D':
                case 'd':
                    oldDirX = dirX;
                    dirX = dirX * cos(0.1) - dirY * sin(0.1);
                    dirY = oldDirX * sin(0.1) + dirY * cos(0.1);
                    oldPlaneX = planeX;
                    planeX = planeX * cos(0.1) - planeY * sin(0.1);
                    planeY = oldPlaneX * sin(0.1) + planeY * cos(0.1);
                    //send_letter_to_buffer(caractereClavier());

                    break;
                default:
                    printf("%c\n", caractereClavier());
                    send_letter_to_buffer(caractereClavier());
                    break;
            }
        case ClavierSpecial:
            break;
        case BoutonSouris:
             switch (etatBoutonSouris()){
                 case GaucheAppuye:
                     if (loopThreadSond == false) {
                         if (strcmp(player.equipped.name, "Gun") == 0 || strcmp(player.equipped.name, "Rifle") == 0) {
                             const char *filename = "music/shot.wav";

                             pthread_t thread;
                             pthread_create(&thread, NULL, play_sound_parallel, (void *) filename);
                             pthread_detach(thread);
                         } else if (strcmp(player.equipped.name, "Dupstep Gun") == 0) {
                             const char *filename = "music/dubstep1.wav";

                             pthread_t thread;
                             pthread_create(&thread, NULL, play_sound_parallel, (void *) filename);
                             pthread_detach(thread);
                         }

                     }
                     break;
             }
            break;
        case Souris:
            break;
        case Inactivite:
            break;
        case Redimensionnement:
            break;
    }
}
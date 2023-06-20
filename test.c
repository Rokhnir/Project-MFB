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
#include "gfxlib/include/ESLib.h"	// Pour utiliser valeurAleatoire()

#define screenWidth 1920
#define screenHeight 1080
#define mapWidth 24
#define mapHeight 24

static char FILENAME[] = {"BCSIntro.wav"};
static bool loopThreadStarted = false;
static bool loopThreadStarted2 = false;
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




void gestionEvenement(EvenementGfx evenement);
void loop();
int worldMap[mapWidth][mapHeight];

void arret_brutal_music(void) {
    char command[256];
    sprintf(command, "killall aplay");
    system(command);
}
void play_sound(const char *filename) {
    char command[256];
    sprintf(command, "aplay %s -d 1", filename);
    system(command);
}
void play_music(void *arg) {
    char command[256];
    sprintf(command, "aplay %s " , FILENAME);
    system(command);
}
void *play_sound_parallel(void *filename) {
    play_sound((const char *)filename);

    pthread_exit(NULL);
}
void send_letter_to_buffer(char letter) {
    int i = 0;
    while (cheatCode[i] != '\0') {
        i++;
    }
    cheatCode[i] = letter;
    cheatCode[i + 1] = '\0';
    if (strcmp(cheatCode, "uWu") == 0) {
        printf("Cheat code sus\n");
        play_sound_parallel("sus.wav");
        cheatCode[0] = '\0';
    }
    if (i > 10) {
        cheatCode[0] = '\0';
    }
    printf("Cheat code %s\n", cheatCode);

}

void* loopThread(void* arg) {
    lanceBoucleEvenements();

    return NULL;
}


//################################# Texture2D ########################################

//####################################################################################

int main(int argc, char **argv)
{
    if (loopThreadStarted == false) {
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
        //pthread_detach(thread2);
        loopThreadStarted2 = true;
    }

    pthread_join(thread2, NULL);

    return main(argc, argv);

// ###########################################

}

void loop()
{

    for (int i = 0; i < 24; i++)
    {
        for (int j = 0; j < 24; j++)
        {
            couleurCourante(50 * worldMap[i][j], 50 * worldMap[i][j], 50 * worldMap[i][j]);
            rectangle((hauteurFenetre() / 24) * (j), (hauteurFenetre() / 24) * (24 - i), (hauteurFenetre() / 24) * (j + 1), (hauteurFenetre() / 24) * (23 - i));
        }
    }

    epaisseurDeTrait(1.f);
    
    int length = planeY;

    for(int i = 0; i < screenWidth; i++){
        double fovX = 2 * i / (double)screenWidth - 1;
        double rayDirX = dirX + planeX * fovX;
        double rayDirY = dirY + planeY * fovX;

        int steps = fmax(abs(rayDirX), abs(rayDirY));
        double incY = (!steps) ? 0 : rayDirX / steps;
        double incX = (!steps) ? 0 : rayDirY / steps;

        double xa = pX, ya = pY;
        int hit = 0;

        while(!hit){
            xa += incX;
            ya += incY;
            int ouiX = floor(xa/(hauteurFenetre()/24));
            int ouiY = floor(ya/(hauteurFenetre()/24));
            if(worldMap[23-ouiY][ouiX] > 0) hit = 1;
            if(hit){
                double test = hauteurFenetre()/24 * ouiX;
                double test2 = hauteurFenetre()/24 * (ouiX+1);
            int value;
            if(((floor(xa) < test+1) && (floor(xa) > test-1)) || ((floor(xa) < test2+1) && (floor(xa) > test2-1))) value = 255;
            else value = 125;
            switch(worldMap[23-ouiY][ouiX]){
                case 1:
                    couleurCourante(value,0,0);
                    break;
                case 2:
                    couleurCourante(0,value,0);
                    break;
                case 3:
                    couleurCourante(0,0,value);
                    break;
                case 4:
                    couleurCourante(value,value,value);
                    break;
                default:
                    couleurCourante(0,value,value);
                    break;
            }}
        }
        double ax = rayDirX - pX;
        double ay = rayDirY - pY;
        double bx = dirX - pX;
        double by = dirY - pY;
        double ab = ax*bx + ay*by;
        double aa = sqrt(pow(ax,2)+pow(ay,2));
        double bb = sqrt(pow(bx,2)+pow(by,2));
        double sino = (ab/(aa*bb));
        int dist = floor(sqrt(pow(xa-pX,2)+pow(ya-pY,2)));
        int lineHeight = screenHeight/dist*20;
        
        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if(drawStart < 0)drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if(drawEnd >= screenHeight)drawEnd = screenHeight - 1;
        //ligne(pX,pY,xa,ya);
        ligne(i,drawStart,i,drawEnd);
    }
    
}

void gestionEvenement(EvenementGfx evenement){

    static bool pleinEcran = false;		  // Pour savoir si on est en mode plein ecran ou pas


    switch (evenement)
    {

    case Initialisation:
        activeGestionDeplacementPassifSouris();
        demandeTemporisation(20);
        dirX = (planeY/0.66);
    dirY = 0;
    pvecX = pX;
    pvecY = 0;
    dvecX = abscisseSouris()-pX;
    dvecY = ordonneeSouris()-pY;
    angle = acos(pvecX*dvecX/(sqrt(pvecX*pvecX)*sqrt(dvecX*dvecX+dvecY*dvecY))) * 180 / 3.14159265;


    break;

    case Temporisation:
        rafraichisFenetre();
        break;

    case Affichage:
        effaceFenetre(0, 0, 0);

        loop();
        break;

    case Clavier:
        switch (caractereClavier())
        {


        case 'X':
        case 'x':
            arret_brutal_music();
            termineBoucleEvenements();
                send_letter_to_buffer(caractereClavier());

                //strcpy(FILENAME, "sus.wav");
            break;
        case 'F':
        case 'f':
            pleinEcran = !pleinEcran;
            if (pleinEcran)
                modePleinEcran();
            else
                redimensionneFenetre(screenWidth, screenHeight);
            send_letter_to_buffer(caractereClavier());

                break;
        case 'Z':
        case 'z':
            pY += hauteurFenetre() * 0.05;
                const char *filename = "pas.wav";

                pthread_t thread;
                pthread_create(&thread, NULL, play_sound_parallel, (void *)filename);
                pthread_detach(thread);
                send_letter_to_buffer(caractereClavier());

                break;
        case 'S':
        case 's':
            pY -= hauteurFenetre() * 0.05;
                send_letter_to_buffer(caractereClavier());

                break;

        case 'Q':
        case 'q':
            oldDirX = dirX;
            dirX = dirX * cos(-0.1) - dirY * sin(-0.1);
            dirY = oldDirX * sin(-0.1) + dirY * cos(-0.1);
            oldPlaneX = planeX;
            planeX = planeX * cos(-0.1) - planeY * sin(-0.1);
            planeY = oldPlaneX * sin(-0.1) + planeY * cos(-0.1);
                send_letter_to_buffer(caractereClavier());

                break;
        case 'D':
        case 'd':
            oldDirX = dirX;
            dirX = dirX * cos(0.1) - dirY * sin(0.1);
            dirY = oldDirX * sin(0.1) + dirY * cos(0.1);
            oldPlaneX = planeX;
            planeX = planeX * cos(0.1) - planeY * sin(0.1);
            planeY = oldPlaneX * sin(0.1) + planeY * cos(0.1);
                send_letter_to_buffer(caractereClavier());

                break;
            default:
                printf("%c\n", caractereClavier());
                send_letter_to_buffer(caractereClavier());
                break;
        }
    case ClavierSpecial:
        break;
    case BoutonSouris:
        // switch (etatBoutonSouris()){}
        break;
    case Souris:
        break;
    case Inactivite:
        break;
    case Redimensionnement:
        break;
    }
}
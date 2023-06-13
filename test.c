#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "gfxlib/include/GfxLib.h"
#include <pthread.h>
#include <string.h>
#include <sndfile.h>
#include <portaudio.h>
#include <stdbool.h>

#define screenWidth 1920
#define screenHeight 1080
#define mapWidth 24
#define mapHeight 24
//#define FILENAME "./sus.wav"
char FILENAME[] = {"BCSIntro.wav"};

/*int worldMap[mapWidth][mapHeight] =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 2, 2, 0, 2, 2, 0, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 5, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 4, 4, 4, 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
*/
void gestionEvenement(EvenementGfx evenement);
void loop();
int worldMap[mapWidth][mapHeight];

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
bool loopThreadStarted = false;
/*void* playSond(void* arg, char* sond);
void play_sound_parallel(const char *filename);*/
/*void play_sound(const char *filename) {
    char command[256];
    sprintf(command, "aplay %s -d 10", filename);
    system(command);
}*/
/*void play_sound(const char *filename) {
    ma_result result;
    ma_engine engine;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        return -1;
    }

    ma_engine_play_sound(&engine, "sus.wav", NULL);

    ma_engine_uninit(&engine);
}



void *play_sound_parallel(void *filename) {
    play_sound((const char *)filename);

    pthread_exit(NULL);
}*/

typedef struct {
    SNDFILE* file;
    SF_INFO fileInfo;
    PaStream* stream;
} SoundData;

void* loopThread(void* arg) {
    lanceBoucleEvenements();

    return NULL;
}


int main(int argc, char **argv)
{
    if (loopThreadStarted == false) {
        /*------------MAP--------------------*/



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
// ###################MUSIC###################
    // Initialisation de PortAudio
    Pa_Initialize();

    // Ouverture du fichier audio avec libsndfile
    SF_INFO fileInfo;
    SNDFILE* file = sf_open(FILENAME, SFM_READ, &fileInfo);
    if (!file) {
        printf("Impossible de charger le fichier audio.\n");
        return 1;
    }

    // Configuration de PortAudio
    PaStream* stream;
    Pa_OpenDefaultStream(&stream, 0, fileInfo.channels, paFloat32, fileInfo.samplerate, paFramesPerBufferUnspecified, NULL, NULL);

    // Démarrage du thread de boucle
    pthread_t thread;
    //pthread_create(&thread, NULL, loopThread, NULL);*/
    if (loopThreadStarted == false) {
        pthread_create(&thread, NULL, loopThread, NULL);
        loopThreadStarted = true;
    }

    // Lecture de la musique
    Pa_StartStream(stream);
    Pa_Sleep(500);  // Temps de démarrage pour éviter les artefacts audio

    // Lecture et conversion du contenu du fichier
    const int BUFSIZE = 1024;
    float buffer[BUFSIZE * fileInfo.channels];
    int readCount;
    while ((readCount = sf_readf_float(file, buffer, BUFSIZE)) > 0) {
        Pa_WriteStream(stream, buffer, readCount);
    }


    // Fermeture du fichier audio et de PortAudio
    sf_close(file);
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
    return main(argc, argv);

    // Attendre la fin du thread
    pthread_join(thread, NULL);

// ###################MUSIC###################
   // lanceBoucleEvenements();
    //return 0;
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

    static bool pleinEcran = false;

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
            //termineBoucleEvenements();
            strcpy(FILENAME, "sus.wav");
            break;
        case 'F':
        case 'f':
            pleinEcran = !pleinEcran;
            if (pleinEcran)
                modePleinEcran();
            else
                redimensionneFenetre(screenWidth, screenHeight);
            break;
        case 'Z':
        case 'z':
            pY += hauteurFenetre() * 0.05;
            break;
        case 'S':
        case 's':
            pY -= hauteurFenetre() * 0.05; 
            break;

        case 'Q':
        case 'q':
            oldDirX = dirX;
            dirX = dirX * cos(-0.1) - dirY * sin(-0.1);
            dirY = oldDirX * sin(-0.1) + dirY * cos(-0.1);
            oldPlaneX = planeX;
            planeX = planeX * cos(-0.1) - planeY * sin(-0.1);
            planeY = oldPlaneX * sin(-0.1) + planeY * cos(-0.1);
            break;
        case 'D':
        case 'd':
            oldDirX = dirX;
            dirX = dirX * cos(0.1) - dirY * sin(0.1);
            dirY = oldDirX * sin(0.1) + dirY * cos(0.1);
            oldPlaneX = planeX;
            planeX = planeX * cos(0.1) - planeY * sin(0.1);
            planeY = oldPlaneX * sin(0.1) + planeY * cos(0.1);
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



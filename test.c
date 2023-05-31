#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "gfxlib/include/GfxLib.h"

#define screenWidth 640
#define screenHeight 480
#define mapWidth 24
#define mapHeight 24

int worldMap[mapWidth][mapHeight] =
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

static double posX = 22, posY = 12;
static double dirX = -1, dirY = 0;
static double planeX = 0;
static double planeY = 0.66;
static double itime = 0;
static double oldTime = 0;
static double frameTime = 0;
static double moveSpeed = 0;
static double rotSpeed = 0;
static double oldDirX = 0;
static int mode;

void gestionEvenement(EvenementGfx evenement);
void test();
void loop();

int main(int argc, char **argv)
{
    scanf("%d", &mode);
    initialiseGfx(argc, argv);
    prepareFenetreGraphique("Doom", screenWidth, screenHeight);
    lanceBoucleEvenements();
    return 0;
}

void test(void)
{
    for (int x = 0; x < screenWidth; x++)
    {
        double cameraX = 2 * x / (double){screenWidth} - 1;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        int mapX = (int){posX};
        int mapY = (int){posY};

        double sideDistX;
        double sideDistY;
        double deltaDistX = (!rayDirX) ? DBL_MAX : abs(1 / rayDirX);
        double deltaDistY = (!rayDirY) ? DBL_MAX : abs(1 / rayDirY);

        double perpWallDist;
        int stepX;
        int stepY;
        int hit = 0;
        int side;

        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (posX + 1.f - mapX) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (posY + 1.f - mapY) * deltaDistY;
        }

        while (!hit)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (worldMap[mapX][mapY] > 0)
                hit = 1;
        }

        if (!side)
            perpWallDist = (sideDistX - deltaDistX);
        else
            perpWallDist = (sideDistY - deltaDistY);

        int lineHeight = (int){screenHeight / perpWallDist};

        int drawStart = (-lineHeight / 2) + (screenHeight / 2);
        if (drawStart < 0)
            drawStart = 0;

        int drawEnd = (lineHeight / 2) + (screenHeight / 2);
        if (drawEnd >= screenHeight)
            drawEnd = screenHeight - 1;

        int value = 255;
        if (side == 1)
            value = 125;

        switch (worldMap[mapX][mapY])
        {
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
            couleurCourante(value, value, 0);
            break;
        }
        ligne(x, drawStart, x, drawEnd);
    }
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

    epaisseurDeTrait(5.f);

    double pX = hauteurFenetre() * 0.6, pY = hauteurFenetre() * 0.1;

    double dirX = abscisseSouris()-pX, dirY = ordonneeSouris()-pY;
    int steps = fmax(abs(dirX), abs(dirY));
    double incX = (!steps) ? 0 : dirX / steps;
    double incY = (!steps) ? 0 : dirY / steps;

    double norme = sqrt(pow(dirX,2) + pow(dirY,2));
    double plan = norme/atan(33*(3.14159265/180));

    couleurCourante(0,0,255);
    //ligne(abscisseSouris()-planX, ordonneeSouris()+planY, abscisseSouris()+planX, ordonneeSouris()-planY);

    double xa = pX, ya = pY;
    int hit = 0;

    while(!hit){
        xa += incX;
        ya += incY;
        int ouiX = floor(xa/(hauteurFenetre()/24));
        int ouiY = floor(ya/(hauteurFenetre()/24));
        if(worldMap[23-ouiY][ouiX] > 0) hit = 1;
    }
    couleurCourante(255,0,0);
    ligne(pX, pY, xa, ya);
}

void gestionEvenement(EvenementGfx evenement)
{

    static bool pleinEcran = false;

    switch (evenement)
    {

    case Initialisation:
        activeGestionDeplacementPassifSouris();
        demandeTemporisation(20);
        break;

    case Temporisation:
        rafraichisFenetre();
        break;

    case Affichage:
        effaceFenetre(0, 0, 0);
        oldTime = itime;
        itime = clock();
        frameTime = (itime - oldTime) / 1000.f;
        moveSpeed = frameTime * 5.0;
        rotSpeed = frameTime * 3.0;
        oldDirX = dirX;
        epaisseurDeTrait(1.f);
        if(mode) test();
        else loop();
        break;

    case Clavier:
        switch (caractereClavier())
        {
        case 'Z':
        case 'z':
            if (worldMap[(int){posX + dirX * moveSpeed}][(int){posY}] == false)
                posX += dirX * moveSpeed;
            if (worldMap[(int){posX}][(int){posY + dirY * moveSpeed}] == false)
                posY += dirY * moveSpeed;
            break;
        case 'S':
        case 's':
            if (worldMap[(int){posX - dirX * moveSpeed}][(int){posY}] == false)
                posX -= dirX * moveSpeed;
            if (worldMap[(int){posX}][(int){posY - dirY * moveSpeed}] == false)
                posY -= dirY * moveSpeed;
            break;
        case 'D':
        case 'd':
            dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
            dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
            double ioldPlaneX = planeX;
            planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
            planeY = ioldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
            break;
        case 'Q':
        case 'q':
            dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
            dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
            planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
            break;
        case 'X':
        case 'x':
            termineBoucleEvenements();
            break;
        case 'F':
        case 'f':
            pleinEcran = !pleinEcran;
            if (pleinEcran)
                modePleinEcran();
            else
                redimensionneFenetre(screenWidth, screenHeight);
            break;
        }
        break;
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
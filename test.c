#include "struct.h"
#include "test.h"
#include <string.h>
#include "outils.h"

static Player player;
Ennemie test;

int main(int argc, char **argv)
{
    player.posx = 11;
    player.posy = 12;
    player.ammo = 30;
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



    /*------------MAP--------------------*/



   /* FILE *file = fopen("map", "r");

    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        return 1;
    }

    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            fscanf(file, "%d", &worldMap[j][i]);
        }
    }

    fclose(file);*/



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
    lanceBoucleEvenements();
    return 0;
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

void iaEnnemie(Ennemie a) {
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

void afficheAmmo(){
    char buff[11], buff1[2];
    sprintf(buff1,"%d",player.equipped.inLoader);
    strcat(buff,buff1);
    strcat(buff, " / ");
    sprintf(buff1,"%d", player.equipped.loader);
    strcat(buff, buff1);
    strcat(buff, " ");
    sprintf(buff1,"%d", player.ammo);
    strcat(buff, buff1);
    couleurCourante(255,0,0);
    epaisseurDeTrait(largeurFenetre()*0.003);
    afficheChaine(buff,largeurFenetre()*0.015 , 0.95 * largeurFenetre(), 50);
}

void afficheHp(int debut, int longeur){
    couleurCourante(128,0,0);
    rectangle(debut,20,debut + longeur,40);
    couleurCourante(0,255,0);
    printf("%f   ", player.life);
    rectangle(debut,20, ((debut + longeur)/ 100) * player.life , 40);
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

        int steps = fmax(fabs(rayDirX), fabs(rayDirY));
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
        afficheAmmo();
        afficheHp(largeurFenetre()/4,largeurFenetre()/2);
        iaEnnemie(test);
        break;

    case Clavier:
        switch (caractereClavier())
        {
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
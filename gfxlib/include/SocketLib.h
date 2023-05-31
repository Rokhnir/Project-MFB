/* Veuillez reporter tout commentaire a ghislain.oudinet@isen.fr */

/*
 *  SocketLib.h
 *  isentlib
 *
 *  Created by Ghislain Oudinet on 27/09/07.
 *
 */

// Sous Windows : linker avec -lwsock32

#ifndef SOCKETLIB_H
#define SOCKETLIB_H

#include <stdbool.h>

// Cree une nouvelle connexion qui pourra ensuite etre branchee sur une adresse et un port particulier
int nouvelleConnexion(void);

// Essaie de brancher une connexion sur une machine dont l'adresse et le port de communication sont donnes
bool ouvreConnexion_Avec_SurPort(int descripteurConnexion, const char *adresseIP, short port);

// Attend qu'une connexion tierce soit etablie sur la machine presente et sur le port donne en parametre
int attendConnexionSurPort(short port);

// Envoie sur une connexion le flux de donnees fourni (attention a l'endianness des donnees)
// Le nombre d'octets ecrits est renvoye
int envoie_DeTaille_Sur(const void *donnees, int longueurDonnees, int descripteurConnexion);

// Receptionne a partir d'une connexion un flux de donnees dans une zone de memoire passee en parametre
// Le nombre d'octets lus est renvoye
int receptionne_DeTaille_Sur(void *donnees, int longueurDonnees, int descripteurConnexion);

// Envoie sur une connexion l'entier fourni (l'endianness est conservee)
// Renvoie faux si la totalite des octets formant l'entier n'a pas ete envoyee
bool envoieEntier_Sur(int entier, int descripteurConnexion);

// Receptionne a partir d'une connexion un entier dans une zone de memoire passee en parametre
// Renvoie faux si la totalite des octets formant l'entier n'a pas ete receptionnee
bool receptionneEntier_Sur(int *entier, int descripteurConnexion);

// Termine la connexion
void fermeConnexion(int descripteurConnexion);

#ifdef WIN32
// A appeler sous Windows avant toute utilisation des sockets
void activeWinsocket(void);

// A appeler sous Windows apres l'utilisation des sockets
void desactiveWinsocket(void);

#endif


#endif

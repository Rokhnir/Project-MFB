/*
 *  SocketLib.c
 *  isentlib
 *
 *  Created by Ghislain Oudinet on 27/09/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#ifndef _WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock.h>
#endif

#include "../include/SocketLib.h"

// Cree une nouvelle connexion qui pourra ensuite etre branchee sur une adresse et un port particulier
int nouvelleConnexion(void)
{
	return socket(PF_INET, SOCK_STREAM, 0); // Ouverture d'un flux de donnees Internet
}

// Essaie de brancher une connexion sur une machine dont l'adresse et le port de communication sont donnes
bool ouvreConnexion_Avec_SurPort(int descripteurConnexion, const char *adresseIP, short port)
{
	struct sockaddr_in adresseConnexion;

	adresseConnexion.sin_family = AF_INET; // La famille d'adressage ciblee est celle d'Internet
	adresseConnexion.sin_port = htons(port); // Le port selectionne
	adresseConnexion.sin_addr.s_addr = inet_addr(adresseIP); // L'adresse selectionne
	memset(adresseConnexion.sin_zero, '\0', sizeof(adresseConnexion.sin_zero)); // Mise a zero obligatoire

	// On essaie de se connecter
	return connect(descripteurConnexion, (struct sockaddr *)&adresseConnexion, sizeof(adresseConnexion)) == 0;
}

// Attend qu'une connexion tierce soit etablie sur la machine presente et sur le port donne en parametre
int attendConnexionSurPort(short port)
{
	struct sockaddr_in adresseLocale;
	struct sockaddr_in adresseClient;
#ifdef _WIN32
	int sockaddr_in_size = sizeof(adresseClient);
#else
	socklen_t sockaddr_in_size = (socklen_t)sizeof(adresseClient);
#endif
	int descripteurConnexion;
	int connexion = 0;

	descripteurConnexion = nouvelleConnexion();
	if (descripteurConnexion > 0)
	{
		adresseLocale.sin_family = AF_INET;; // La famille d'adressage ciblee est celle d'Internet
		adresseLocale.sin_port = htons(port); // Le port selectionne
		adresseLocale.sin_addr.s_addr = htonl(INADDR_ANY); // Choix automatique de l'adresse IP locale
		memset(adresseLocale.sin_zero, '\0', sizeof(adresseLocale.sin_zero)); // Mise a zero obligatoire

		if (bind(descripteurConnexion, (struct sockaddr *)&adresseLocale, sizeof(adresseLocale)) != -1) // On autorise le referencement de la connexion
		{
			if (listen(descripteurConnexion, 1) == 0) // Si on entend quelque chose lors de l'ecoute sur une seule et unique connexion
				connexion = accept(descripteurConnexion, (struct sockaddr *)&adresseClient, &sockaddr_in_size); // On accepte le flux d'informations qui en arrive
		}
		fermeConnexion(descripteurConnexion);
	}
	return connexion;
}

// Envoie sur une connexion le flux de donnees fourni (attention a l'endianness des donnees)
// Le nombre d'octets ecrits est renvoye
int envoie_DeTaille_Sur(const void *donnees, int longueurDonnees, int descripteurConnexion)
{
	return send(descripteurConnexion, donnees, longueurDonnees, 0);
}

// Receptionne a partir d'une connexion un flux de donnees dans une zone de memoire passee en parametre
// Le nombre d'octets lus est renvoye
int receptionne_DeTaille_Sur(void *donnees, int longueurDonnees, int descripteurConnexion)
{
	return recv(descripteurConnexion, donnees, longueurDonnees, 0);
}

// Envoie sur une connexion l'entier fourni (l'endianness est conservee)
// Renvoie faux si la totalite des octets formant l'entier n'a pas ete envoyee
bool envoieEntier_Sur(int entier, int descripteurConnexion)
{
	entier = htonl(entier);
	return sizeof(entier) == envoie_DeTaille_Sur(&entier, sizeof(entier), descripteurConnexion); // Erreur si la taille envoyee n'est pas egale a celle de la donnee
}

// Receptionne a partir d'une connexion un entier dans une zone de memoire passee en parametre
// Renvoie faux si la totalite des octets formant l'entier n'a pas ete receptionnee
bool receptionneEntier_Sur(int *entier, int descripteurConnexion)
{
	int tailleLue = receptionne_DeTaille_Sur(entier, sizeof(int), descripteurConnexion);
	*entier = ntohl(*entier);
	return sizeof(int) == tailleLue; // Erreur si la taille lue n'est pas celle attendue
}

// Termine la connexion
void fermeConnexion(int descripteurConnexion)
{
#ifndef _WIN32
	close(descripteurConnexion);
#else
	closesocket(descripteurConnexion);
#endif
}

#ifdef _WIN32
// A appeler sous Windows avant toute utilisation des sockets
void activeWinsocket(void)
{
     WSADATA wsaData;
     if (WSAStartup(MAKEWORD(1, 1), &wsaData)) exit(1);
}

// A appeler sous Windows apres l'utilisation des sockets
void desactiveWinsocket(void)
{
     WSACleanup();
}

#endif

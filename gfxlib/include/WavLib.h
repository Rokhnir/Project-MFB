/* Veuillez reporter tout commentaire a ghislain.oudinet@isen.fr */

#ifndef WAVLIB_H
#define WAVLIB_H

#include <stdbool.h>

typedef struct
{
	int frequenceEchantillonnage; // Frequence de reproduction du fichier sonore
	int nombreDeCanaux; // Nombre de canaux entrelaces
	int tailleEchantillon; // Taille en octets d'un echantillon
	int nombreEchantillonnages; // Nombre d'echantillonnages composant le son
		// (nombreEchantillonnages * nombreDeCanaux = nombre d'echantillons)
	void *donneesSonores; // Donnees rangees par numero d'echantillon, puis pour chaque echantillon par canal
} DonneesSonWAV;

// Fonction s'occupant de la liberation complete d'une structure DonneesSonWAV
void libereDonneesSonWAV(DonneesSonWAV **structure);

// Fonction essayant de lire le fichier passe en parametre, et renvoyant une structure
//	contenant les informations du fichier sonore en cas de succes, NULL sinon
DonneesSonWAV *lisWAV(char *nom);

// Fonction ecrivant les informations du son dans le fichier passe en parametre.
//	Renvoie faux en cas de probleme, vrai sinon
bool ecrisSonWAV_dans(DonneesSonWAV *donneesSonWAV, char *nom);

#endif

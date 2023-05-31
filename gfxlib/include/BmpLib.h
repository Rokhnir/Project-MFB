/* Veuillez reporter tout commentaire a ghislain.oudinet@isen.fr */

#ifndef BMPLIB_H
#define BMPLIB_H

#include <stdbool.h>

/* Partie publique elaboree
   Cette partie est relative a toutes les informations utiles pour
   personnaliser et mieux controler l'interaction avec les fichiers BMP */

typedef struct
{
	int largeurImage;
	int hauteurImage;
	unsigned char *donneesRGB; /* Donnees sauvees dans l'ordre bleu-vert-rouge */
} DonneesImageRGB;

/* Fonction s'occupant de la liberation complete d'une structure DonneesImageRGB */
void libereDonneesImageRGB(DonneesImageRGB **structure);

/* Fonction essayant de lire le fichier passe en parametre, et renvoyant une structure
	contenant les informations de l'image en cas de succes, NULL sinon */
DonneesImageRGB *lisBMPRGB(char *nom);

/* Fonction ecrivant les informations de l'image dans le fichier passe en parametre.
	Renvoie faux en cas de probleme, vrai sinon */
bool ecrisBMPRGB_Dans(DonneesImageRGB *donneesImage, char *nom);

#endif

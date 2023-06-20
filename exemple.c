#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h>	// Pour pouvoir utiliser printf()
#include <math.h>	// Pour pouvoir utiliser sin() et cos()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h"	// Pour utiliser valeurAleatoire()

// Largeur et hauteur par defaut d'une image correspondant a nos criteres
#define LargeurFenetre 800
#define HauteurFenetre 600
DonneesImageRGB *image = NULL;
int *pixels = NULL;
Texture2D *texture = NULL;

/* La fonction de gestion des evenements, appelee automatiquement par le systeme
des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement);

static int *BVR2ARVB(int largeur, int hauteur, const unsigned char *donnees)
{
	const unsigned char *ptrDonnees;
	unsigned char *pixels = (unsigned char *)malloc(largeur * hauteur * sizeof(int));
	unsigned char *ptrPixel;
	int index;
	ptrPixel = pixels;
	ptrDonnees = donnees;
	for (index = largeur * hauteur; index != 0; --index) /* On parcourt tous les
														  pixels de l'image */
	{
		ptrPixel[0] = ptrDonnees[0];
		ptrPixel[1] = ptrDonnees[1];
		ptrPixel[2] = ptrDonnees[2];
		ptrPixel[3] = 0x0FF;
		ptrDonnees += 3; /* On passe a la premiere composante du pixel suivant */
		ptrPixel += 4;	 /* On passe au pixel suivant */
	}
	return (int *)pixels;
}

void supprimerCouleur(Texture2D *texture)
{
	int *pixels = texture->donnees;
	int largeur = texture->largeur;
	int hauteur = texture->hauteur;

	for (int i = 0; i < largeur * hauteur; i++)
	{
		// Extraire les composantes RGBA du pixel
		int rouge = (pixels[i] >> 24) & 255;
		int vert = (pixels[i] >> 16) & 255;
		int bleu = (pixels[i] >> 8) & 255;
		int alpha = pixels[i] & 255;

		// Vérifier si le pixel correspond à la couleur à supprimer (R, V, B)
		if (rouge == 255 && vert == 0 && bleu == 255)
		{
			// Mettre l'opacité à 0
			alpha = 0;
		}

		// Reconstruire le pixel modifié
		pixels[i] = (rouge << 24) | (vert << 16) | (bleu << 8) | alpha;
	}

	// Mettre à jour la texture avec les nouveaux pixels
	modifieTexture2D(texture, pixels);
}

int main(int argc, char **argv)
{
	initialiseGfx(argc, argv);

	prepareFenetreGraphique("OpenGL", LargeurFenetre, HauteurFenetre);

	/* Lance la boucle qui aiguille les evenements sur la fonction gestionEvenement ci-apres,
		qui elle-meme utilise fonctionAffichage ci-dessous */
	lanceBoucleEvenements();

	return 0;
}

/* La fonction de gestion des evenements, appelee automatiquement par le systeme
des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement)
{
	static bool pleinEcran = false;		  // Pour savoir si on est en mode plein ecran ou pas
	static DonneesImageRGB *image = NULL; // L'image a afficher au centre de l'ecran

	switch (evenement)
	{
	case Initialisation:
		image = lisBMPRGB("/home/allow/Downloads/gfxlib/image/npc.bmp");
		pixels = BVR2ARVB(image->largeurImage, image->hauteurImage, image->donneesRGB);
		texture = creeTexture2D(image->largeurImage, image->hauteurImage, pixels);

		// Configure le système pour générer un message Temporisation
		// toutes les 20 millisecondes
		demandeTemporisation(20);
		break;

	case Temporisation:
		// il faut redessiner la fenetre :
		rafraichisFenetre();
		break;

	case Affichage:
		effaceFenetre(255, 255, 255);

		if (image != NULL) // Si l'image a pu être lue
		{
			// Afficher la texture avec la fonction rectangleSelonTexture
			rectangleSelonTexture((largeurFenetre() / 2) - 32, hauteurFenetre() / 8, texture);
		}
		else // Si l'image n'a pas pu être lue
		{
			// Afficher un message d'erreur
			couleurCourante(255, 0, 0);
			afficheChaine("Impossible de lire l'image", 20, 20, 20);
		}

		break;

	case Clavier:
		printf("%c : ASCII %d\n", caractereClavier(), caractereClavier());

		switch (caractereClavier())
		{
		case 'Q': /* Pour sortir quelque peu proprement du programme */
		case 'q':
			libereDonneesImageRGB(&image); /* On libere la structure image,
			c'est plus propre, meme si on va sortir du programme juste apres */
			termineBoucleEvenements();
			break;

		case 'F':
		case 'f':
			pleinEcran = !pleinEcran; // Changement de mode plein ecran
			if (pleinEcran)
				modePleinEcran();
			else
				redimensionneFenetre(LargeurFenetre, HauteurFenetre);
			break;

		case 'R':
		case 'r':
			// Configure le systeme pour generer un message Temporisation
			// toutes les 20 millisecondes (rapide)
			demandeTemporisation(20);
			break;

		case 'L':
		case 'l':
			// Configure le systeme pour generer un message Temporisation
			// toutes les 100 millisecondes (lent)
			demandeTemporisation(100);
			break;

		case 'S':
		case 's':
			// Configure le systeme pour ne plus generer de message Temporisation
			demandeTemporisation(-1);
			break;
		}
		break;

	case ClavierSpecial:
		printf("ASCII %d\n", toucheClavier());
		break;

	case BoutonSouris:
		switch (etatBoutonSouris())
		{
		case GaucheAppuye:
			printf("Bouton gauche appuye en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
			break;
		case GaucheRelache:
			printf("Bouton gauche relache en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
			break;
		case DroiteAppuye:
		case DroiteRelache:
			puts("Bouton droite");
			break;
		case MilieuAppuye:
		case MilieuRelache:
			puts("Bouton milieu");
			break;
		case ScrollDown:
			puts("Scroll down");
			break;
		case ScrollUp:
			puts("Scroll up");
			break;
		case ScrollRight:
			puts("Scroll right");
			break;
		case ScrollLeft:
			puts("Scroll left");
			break;
		}
		break;

	case Souris: // Si la souris est deplacee
		break;

	case Inactivite: // Quand aucun message n'est disponible
		break;

	case Redimensionnement: // La taille de la fenetre a ete modifie ou on est passe en plein ecran
		// Donc le systeme nous en informe
		printf("Largeur : %d\t", largeurFenetre());
		printf("Hauteur : %d\n", hauteurFenetre());
		break;
	}
}

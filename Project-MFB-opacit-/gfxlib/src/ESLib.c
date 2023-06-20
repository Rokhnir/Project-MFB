/* Veuillez reporter tout commentaire a ghislain.oudinet@isen.fr */

/*
 
	Version 1.9 : ajout de tempsReel

	Version 1.8 : ajout de valeurIntervalleZeroUn pour sortir une valeur
		aleatoire entre 0 et 1 inclus

	Version 1.7 : modification de lisCaractere de maniere similaire a lisEntier
		et lisFlottant ; cela permet de lire un caractere valide suivi d'un retour
		chariot, mais rend moins aisee la detection d'un appui sur une touche clavier.
		Cependant, ce comportement semble preferable a l'avenir pour les TP d'algo.
		
	Version 1.6 : correction de lisEntier et de lisFlottant de maniere a ce que
		tous les systemes detectent l'entree d'une valeur invalide et affiche le
		message adequat.
	
	Version 1.5 : lisPrecision rendue publique
	
	Version 1.4 : suppression des fflush(stdin).

	Version 1.3 : remplacement de BIBLIOES_H par ESLIB_H dans ESLib.h.
	Ajout de la constante NombreLignesEffacageEcran pour fixer le nombre de
	lignes simulant l'effacement de l'ecran.
	Ajout de la fonction lisPrecision mais uniquement pour des raisons de
	deboguage, sans echo dans ESLib.h.
	
	Version 1.2 : remplacement de time(NULL) par (time(NULL)&0x07FFFFF)
	dans valeurAleatoire() pour eviter que les trops grandes valeurs de
	time() ne produisent des suites similaires sur plus d'une seconde.
	Prise en compte de l'affichage d'un message d'erreur lors de la lecture au
	clavier d'une valeur.
	Ajout des fflush apres chaque sortie texte sur l'ecran
	
	Version 1.1 : ajout de lisChaine_deTaille.
	precisionAffichage vaut -1 par defaut.	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include "../include/ErreurLib.h"
#include "../include/ESLib.h"

/* La variable permettant de controler le nombre de chiffres a afficher
   apres la virgule pour un nombre en virgule flottante */
static int precisionAffichage = -1;

/* Le nombre de sauts de lignes pour simuler l'effacement d'un ecran */
static const int NombreLignesEffacageEcran = 100;

/* La graine servant a initialiser le generateur aleatoire */
static unsigned long graineGeneAleatoire = 0UL;

/* Lis une valeur entiere entree au clavier et la renvoie */
int lisEntier(void)
{
	char ligne[80];
	int entier;
	
	while (fgets(ligne, 80, stdin) == NULL || sscanf(ligne, "%d", &entier) != 1)
		printf("%s", ChaineErreurLisEntier);

	return entier;
}

/* Lis une valeur flottante entree au clavier et la renvoie */
float lisFlottant(void)
{
	char ligne[80];
	float flottant;
	
	while (fgets(ligne, 80, stdin) == NULL || sscanf(ligne, "%f", &flottant) != 1)
		printf("%s", ChaineErreurLisFlottant);

	return flottant;
}

/* Lis un caractere au clavier et le renvoie */
char lisCaractere(void)
{
	char ligne[80];
	char caractere;
	
	while (fgets(ligne, 80, stdin) == NULL || sscanf(ligne, "%c", &caractere) != 1)
		printf("%s", ChaineErreurLisCaractere);

	return caractere;
}

/* Lis une chaine de caracteres de longueur donnee */
void lisChaine_deTaille(char *chaine, int taille)
{
	char *ptrFinLigne;
	
	if (fgets(chaine, taille, stdin) != NULL)
	{
		if ((ptrFinLigne = strpbrk(chaine, "\n\r")) != NULL)
		{
			*ptrFinLigne = '\0'; /* Remplace le premier caractere de fin de
			ligne par un caractere de fin de chaine */
		}
		else
		{
			chaine[taille-1] = '\0';
		}
	}
	else
		chaine[0] = '\0';
}

/* Ecris une chaine de caracteres sur l'ecran */
void ecrisChaine(char *chaine)
{
	printf("%s", chaine); // "%s" to avoid a possible nasty and unuseful warning
	fflush(stdout);
}

/* Ecris un entier decimal sur l'ecran */
void ecrisEntier(int valeur)
{
	printf("%d", valeur);
	fflush(stdout);
}

/* Lis le nombre de chiffres a afficher apres la virgule pour un flottant */
int lisPrecision(void)
{
	return precisionAffichage;
}

/* Fixe le nombre de chiffres a afficher apres la virgule pour un flottant */
void fixePrecision(int n)
{
	precisionAffichage = n;
}

/* Ecris un flottant sur l'ecran */
void ecrisFlottant(float valeur)
{
	if (precisionAffichage >= 0)
	{
		printf("%.*f", precisionAffichage, valeur);
	}
	else
	{
		printf("%g", valeur);
	}
	fflush(stdout);
}

/* Ecris un caractere sur l'ecran */
void ecrisCaractere(char valeur)
{
	printf("%c", valeur);
	fflush(stdout);
}

/* Insere une tabulation */
void tabulation(void)
{
	printf("\t");
	fflush(stdout);
}

/* Passe a la ligne suivante */
void sautDeLigne(void)
{
	puts("");
	fflush(stdout);
}

/* Efface l'ecran */
void effaceEcran(void)
{
	int indexLigne;
	
	for (indexLigne = 0; indexLigne < NombreLignesEffacageEcran; ++indexLigne)
	{
		sautDeLigne();
	}
}

#ifndef _WIN32
/* Renvoie le temps reel depuis le 1er janvier 1970 */
double tempsReel(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (double)tv.tv_sec+(double)tv.tv_usec/1000000.;
}
#else
/* Renvoie le temps reel depuis le demarrage du systeme */
double tempsReel(void)
{
	return (double)GetTickCount()/1000.;
}

#endif

/* Renvoie le temps CPU en secondes relatif au programme */
double tempsCPU(void)
{
	return (double)(unsigned)clock()/(double)CLOCKS_PER_SEC;
}

/* Initialise le generateur aleatoire avec la valeur passee en parametre */
void initialiseValeurAleatoire(float valeur)
{
	unsigned long temps;
	
	temps = (unsigned long)valeur*3141592621UL;
	srand(temps);
	graineGeneAleatoire = temps;
}

/* Renvoie une valeur aleatoire comprise entre 0 inclus et 1 exclu */
float valeurAleatoire(void)
{
	if (graineGeneAleatoire == 0UL)
	{
		initialiseValeurAleatoire((float)(time(NULL)&0x07FFFFF));
	}
	return (float)rand()/((float)RAND_MAX+1);
}

/* Renvoie une valeur aleatoire comprise entre 0 inclus et 1 inclus */
float valeurIntervalleZeroUn(void)
{
	if (graineGeneAleatoire == 0UL)
	{
		initialiseValeurAleatoire((float)(time(NULL)&0x07FFFFF));
	}
	return (float)rand()/((float)RAND_MAX);
}


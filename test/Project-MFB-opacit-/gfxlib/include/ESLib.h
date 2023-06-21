/* Veuillez reporter tout commentaire a ghislain.oudinet@isen.fr */

/*

	Version 1.9 : ajout de tempsReel()
	Version 1.8 : ajout de valeurIntervalleZeroUn pour sortir une valeur
		aleatoire entre 0 et 1 inclus
	Version 1.5 : lisPrecision rendue publique
	Version 1.1 : ajout de lisChaine_deTaille
*/

#ifndef ESLIB_H
#define ESLIB_H

/* Lis une valeur entiere entree au clavier et la renvoie */
int lisEntier(void);

/* Lis une valeur flottante entree au clavier et la renvoie */
float lisFlottant(void);

/* Lis un caractere au clavier et le renvoie */
char lisCaractere(void);

/* Lis une chaine de caracteres de longueur donnee */
void lisChaine_deTaille(char *chaine, int taille);

/* Ecris une chaine de caracteres sur l'ecran */
void ecrisChaine(char *chaine);

/* Ecris un entier decimal sur l'ecran */
void ecrisEntier(int valeur);

/* Fixe le nombre de chiffres a afficher apres la virgule pour un flottant */
void fixePrecision(int n);

/* Lis le nombre de chiffres a afficher apres la virgule pour un flottant */
int lisPrecision(void);

/* Ecris un flottant sur l'ecran */
void ecrisFlottant(float valeur);

/* Ecris un caractere sur l'ecran */
void ecrisCaractere(char valeur);

/* Insere une tabulation */
void tabulation(void);

/* Passe a la ligne suivante */
void sautDeLigne(void);

/* Efface l'ecran */
void effaceEcran(void);

/* Renvoie une information temps reel */
double tempsReel(void);

/* Renvoie le temps CPU en secondes relatif au programme */
double tempsCPU(void);

/* Initialise le generateur aleatoire avec la valeur passee en parametre */
void initialiseValeurAleatoire(float valeur);

/* Renvoie une valeur aleatoire comprise entre 0 inclus et 1 exclu */
float valeurAleatoire(void);

/* Renvoie une valeur aleatoire comprise entre 0 inclus et 1 inclus */
float valeurIntervalleZeroUn(void);

#endif

// Veuillez reporter tout commentaire a ghislain.oudinet@yncrea.fr


//	Version 3.3 (22/06/2022) : ajout du renvoi de l'ancienne épaisseur par epaisseurDeTrait()
//								ajout du support du bouton du milieu ainsi que du scroll
//
//  Version 3.2 (16/09/2016) : remplacement de demandeAnimation_ips() par un demandeTemporisation() plus générique (qui envoie le message Temporisation)
//		Ajout de la fonction termineBoucleEvenements(), necessaire dorenavant sous FreeGLUT sous Linux pour faire quelques nettoyages avant fin du programme
//		Les actions sur le clavier ne memorisent plus les coordonnees de la souris (a moins de recompiler en activant ACTION_CLAVIER_MEMORISE_POSITION_SOURIS)
//
//	Version 3.1 (19/12/2009) : modification de toucheShiftAppuyee, toucheCtrlAppuyee et toucheAltAppuyee pour qu'elles
//      renvoient l'etat en temps reel, et plus seulement lors du dernier evenement concerne (permet plus de possibilites lors d'animations)
//      homogeneisation de l'ecriture du code du programme principal avec l'equivalence de initialisationGLUT
//      	et initialiationGDI, et utilisation de _main sous Win32 au lieu de main
//		ajout de la fonction messageErreur
//
//	Version 3 (02/11/2009) : simplification de la bibliotheque sous une forme specifique (mais plus limitee, sans 3D) pour compatibilite Windows
//
//	Version 2.6 (12/02/2009) : utilisation d'un GL_QUADS dans la fonction rectangle au lieu de glRectf
//		ajout de la fonction BVR2ARVB
//		ajout de la fonction BVR2ABVR
//		ajout de la fonction creeTexture2D
//		ajout de la fonction rectangleSelonTexture
//		ajout de la fonction libereTexture
//
//	Version 2.5.1 (28/03/2008) : ajout de l'opacite dans le choix de la couleur
//        courante, avec la fonction couleurCouranteOpaque
//
//	Version 2.5 (21/08/2007) : ajout de la gestion des touches speciales du clavier
//		ajout de ecrisImageARVB
//
//	Version 2.4.1 (23/01/07) : ajout de materiauLuminescent
//
//	Version 2.4 (16/01/2007) : ajout de la gestion du message Redimensionnement
//		ajout des fonctions largeurFenetre et hauteurFenetre
//		ajout de modifieFonctionDeplacementPassifSouris et de activeGestionDeplacementPassifSouris
//		ajout de fonctionDeplacementPassifSouris
//		ajout de modePleinEcran et de redimensionneFenetre
//
//	Version 2.3 (23/10/2006) : ajout des fonctions Timer et du redessin programme
//	
//	Version 2.2 (30/06/2006) : prepareFenetreGraphique fait cycler la position
//		de depart des fenetres
//	
//	Version 2.1 : main est sorti de la bibliotheque, pour des besoins pedagogiques,
//		cassant la compatibilite avec les anciens codes. Cependant, on peut reintegrer
//		le main manquant dans chaque programme, a partir du code suivant :
//int main(int argc, char **argv)
//{	initialiseGLUT(argc, argv);
//	gfxMain();
//	return 0;}
//
//	Version 2.0 (29/01/06) : ajout de la gestion de la 3D
//		modification de fonctionRedimensionnement, ajout de sAngleDeVue et sVisualisationMode2D
//		ajout de prepareFenetre_En_DeTaille
//		ajout de passeEnMode3D, passeEnMode2D
//		definitions de Point3DGfx et de Vecteur3DGfx
//		ajout de fixeLuminositeAmbiante, lumiereDiffuse_Couleur_En et placeObservateur
//		ajout de point3D, ligne3D et triangle3D
//		ajout de empileTranslation, empileRotationSelonX, empileRotationSelonY et
//			empileRotationSelonZ
//
//	Version 1.2 : ajout de fonctions d'affichage et de lecture ecran d'images bitmap
//
//	Version 1.1.1 : correction de rafraichitFenetre en rafraichisFenetre
//
//	Version 1.1 : affichage des erreurs OpenGL dans fonctionAffichage
//		afficheImage en cours de developpement

#ifndef GFXLIB_H
#define GFXLIB_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdbool.h>
/*#define bool int
#define true (0==0)
#define false (!true)*/



/* Partie publique basique
   Cette partie est relative a toutes les informations utiles pour
   mettre en place simplement un environnement graphique portable */


typedef enum {Inactivite, Affichage, Clavier, ClavierSpecial, Souris, BoutonSouris, Initialisation, Redimensionnement, Temporisation}
	EvenementGfx;
typedef enum {GaucheAppuye, GaucheRelache, DroiteAppuye, DroiteRelache, MilieuAppuye, MilieuRelache, ScrollDown, ScrollUp, ScrollRight, ScrollLeft}
	EtatBoutonSouris;
typedef enum {SuiteDePoints, SuiteDeLignes, SuiteDeTriangles, SuiteDeRectangles, BandeDeTriangles, BandeDeRectangles}
	PrimitiveGfxLib;
typedef enum {ToucheF1 = 1, ToucheF2, ToucheF3, ToucheF4, ToucheF5, ToucheF6, ToucheF7, ToucheF8, ToucheF9, ToucheF10, ToucheF11, ToucheF12,
	ToucheFlecheHaut, ToucheFlecheBas, ToucheFlecheGauche, ToucheFlecheDroite, ToucheNonGeree} TouchesSpeciales;

typedef struct
	{
		int largeur; // Largeur des donnees
		int hauteur; // Hauteur des donnees
		float proportionX;
		float proportionY;
		int *donnees; // Les donnees de la texture
		unsigned int numero; // L'identifiant OpenGL de la texture
	} Texture2D;


// Pour initialiser la partie graphie de la bibliotheque
void initialiseGfx(int argc, char **argv);

/* Prepare une fenetre en specifiant son nom, sa largeur et sa hauteur.
   La fenetre presente un repere orthonorme classique (premier quadrant),
   de largeur pixels sur hauteur pixels */
void prepareFenetreGraphique(const char *nom, int largeur, int hauteur);
/* Lance la boucle de gestion des evenements */
void lanceBoucleEvenements(void);
/* Termine le programme proprement */
void termineBoucleEvenements(void);


/* Fonction de gestion des evenements graphiques, a definir par l'utilisateur */
extern void gestionEvenement(EvenementGfx evenement);



/* Demande la mise a jour du contenu graphique de la fenetre, en lancant un message d'affichage */
void rafraichisFenetre(void);

/* Efface la fenetre avec la couleur donnee
	Les composantes rouge, vert et bleu s'etendent de 0 a 255 inclus */
void effaceFenetre(int rouge, int vert, int bleu);

/* Selectionne la couleur courante de dessin
	Les composantes rouge, vert et bleu s'etendent de 0 a 255 inclus */
void couleurCourante(int rouge, int vert, int bleu);

// Definit l'epaisseur de trait EN PIXELS DE LA FENETRE servant a afficher les points et les lignes
// Renvoie la dernière épaisseur de trait utilisée
float epaisseurDeTrait(float epaisseur);

/* Dessine un point de couleur courante aux coordonnees donnees */
void point(float x, float y);

/* Dessine une ligne de couleur courante aux coordonnees donnees */
void ligne(float xDepart, float yDepart, float xArrivee, float yArrivee);

// Dessine un triangle de couleur courante aux coordonnees donnees
void triangle(float xCoin1, float yCoin1, float xCoin2, float yCoin2, float xCoin3, float yCoin3);

/* Dessine un rectangle de couleur courante aux coordonnees donnees */
void rectangle(float xCoin1, float yCoin1, float xCoin2, float yCoin2);

/* Affiche une chaine de caracteres de taille donnee a la position specifiee */
void afficheChaine(const char *chaine, float taille, float x, float y);

/* Renvoie l'espace occupe par la chaine de caracteres de taille donnee */
float tailleChaine(const char *chaine, float taille);

/* Envoie un message Affichage apres la duree specifiee */
void demandeRedessinDans_ms(int millisecondes);

// Demarre l'envoi periodique de messages Temporisation
void demandeTemporisation(int millisecondesEntreAppels);


/* Lis une portion de la fenetre, couvrant largeur*hauteur et demarrant en (x, y)
Les donnees lues sont sauvees comme une succession de valeurs B, V, R de type
unsigned char */
void lisImage(int x, int y, int largeur, int hauteur, unsigned char *donnees);

// Ecris dans la fenetre une image BVR couvrant largeur*hauteur et demarrant en (x, y) */
void ecrisImage(int x, int y, int largeur, int hauteur, const unsigned char *donnees);

#ifndef _WIN32
// Ecris une image couvrant largeur*hauteur, demarrant en (x, y) et faite de
// pixels ARVB (stockes dans des int little endian) dans la fenetre */
void ecrisImageARVB(int x, int y, int largeur, int hauteur, const int *donneesARVB);

// Cree une texture reutilisable de largeur et hauteur donnees, faite de pixels ABVR little endian
Texture2D *creeTexture2D(int largeur, int hauteur, const int *pixelsABVR);
// Permet de modifier les pixels de la texture prealablement definie, en donnant un nouveau tableau de pixels ABVR little endian
void modifieTexture2D(const Texture2D *texture, const int *pixelsABVR);
// Affiche la texture passee en parametre aux coordonnees fournies
void rectangleSelonTexture(float x, float y, const Texture2D *texture);
// Libere les ressources associees a la gestion de la texture
void libereTexture(Texture2D **texture);
#endif


/* Renvoie l'abscisse de souris correspondant au dernier evenement Clavier, Souris ou BoutonSouris */
int abscisseSouris(void);
/* Renvoie l'ordonnee de souris correspondant au dernier evenement Clavier, Souris ou BoutonSouris */
int ordonneeSouris(void);
/* Renvoie le dernier etat de bouton souris traite par l'evenement BoutonSouris */
EtatBoutonSouris etatBoutonSouris(void);
/* Renvoie le dernier caractere clavier traite par l'evenement Clavier */
char caractereClavier(void);
/* Renvoie la derniere touche speciale du clavier traitee par l'evenement ClavierSpecial */
int toucheClavier(void);

/* Indique l'etat de la touche Shift lors du dernier evenement Clavier */
bool toucheShiftAppuyee(void);
/* Indique l'etat de la touche Ctrl lors du dernier evenement Clavier */
bool toucheCtrlAppuyee(void);
/* Indique l'etat de la touche Alt lors du dernier evenement Clavier */
bool toucheAltAppuyee(void);


// Renvoie la largeur actuelle de la fenetre
int largeurFenetre(void);
// Renvoie la hauteur actuelle de la fenetre
int hauteurFenetre(void);
// Demande au systeme de redimensionner la fenetre
void redimensionneFenetre(int largeur, int hauteur);
// Demande au systeme de passer la fenetre en plein ecran
void modePleinEcran(void);
// Affiche un message d'erreur de maniere standard pour l'OS
void messageDErreur(const char *message);


/* Active la gestion du deplacement passif de la souris */
void activeGestionDeplacementPassifSouris(void);



/* Partie publique elaboree
   Cette partie est relative a toutes les informations utiles pour
   personnaliser et mieux controler l'environnement graphique de base */


#endif

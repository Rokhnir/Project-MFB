// Veuillez reporter tout commentaire a ghislain.oudinet@isen.fr

//	Version 3.3 (22/06/2022) : ajout du renvoi de l'ancienne épaisseur par epaisseurDeTrait()
//								ajout du support du bouton du milieu ainsi que du scroll
//
//	Version 3.2 (16/09/2016) : remplacement de demandeAnimation_ips() par un demandeTemporisation() plus générique (qui envoie le message Temporisation)
//		Ajout de la fonction termineBoucleEvenements(), necessaire dorenavant sous FreeGLUT sous Linux pour faire quelques nettoyages avant fin du programme
//		Les actions sur le clavier ne memorisent plus les coordonnees de la souris (a moins de recompiler en activant ACTION_CLAVIER_MEMORISE_POSITION_SOURIS)
//
//	Version 3.1 (19/12/2009) : modification de toucheShiftAppuyee, toucheCtrlAppuyee et toucheAltAppuyee pour qu'elles
//            renvoient l'etat en temps reel, et plus seulement lors du dernier evenement concerne (permet plus de possibilites lors d'animations)
//            homogeneisation de l'ecriture du code du programme principal avec l'equivalence de initialisationGLUT
//                et initialiationGDI, et utilisation de _main sous Win32 au lieu de main
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

#ifdef _WIN32
	#include <windows.h>
#endif

#ifdef __APPLE__
//	#include <OpenGL/gl.h>
//	#include <OpenGL/CGLMacro.h>
	#include <GLUT/glut.h>		// Header File For The GLut Library
#else
	#include <GL/glut.h>			// Header File For The GLut Library
	#ifdef _WIN32
		#include <GL/glext.h>
	#else
		#include <X11/Xlib.h>
		#include <GL/glx.h>
	#endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/GfxLib.h"
#include "../include/ESLib.h" // Pour tempsReel

//#define DEBUG_MODE

//#define ACTION_CLAVIER_MEMORISE_POSITION_SOURIS

/* Type des fonctions callback que GLUT appelle */
typedef void (*FonctionVide)(void);
typedef void (*FonctionDeuxInts)(int a, int b);
typedef void (*FonctionTroisInts)(int a, int b, int c);
typedef void (*FonctionQuatreInts)(int a, int b, int c, int d);
typedef void (*FonctionCharDeuxInts)(unsigned char a, int b, int c);

/* La constante Pi */
// static const double Pi = 3.14159265358979323846;

/* Memorise le dernier caractere clavier qui a ete genere */
static GLubyte sCaractereClavier = ' ';
/* Memorise la derniere touche speciale du clavier qui a ete pressee */
static int sToucheClavier = ' ';
/* Memorise la derniere action sur un bouton de la souris */
static EtatBoutonSouris sEtatBoutonSouris = GaucheRelache;
/* Memorise l'abscisse courante de la souris */
static int sAbscisseSouris = 0;
/* Memorise l'ordonnee courante de la souris */
static int sOrdonneeSouris = 0;
// Memorise l'appui de la touche Shift
//static bool sAppuiToucheShift = false;
// Memorise l'appui de la touche Ctrl
//static bool sAppuiToucheCtrl = false;
// Memorise l'appui de la touche Alt
//static bool sAppuiToucheAlt = false;
/* Booleen indiquant si la fonction lanceBoucleEvenements a ete appelee */
static bool boucleEvenementsLancee = false;
// Memorisation de l'intervalle en ms entre deux evenements timer
static volatile int sMillisecondesEntreAppels = -1;
// Identifiant de la fenetre graphique
static int sIDWindow = -1;

/* Memorise la taille de la fenetre en cours de gestion */
static int sLargeurFenetre = 0;
static int sHauteurFenetre = 0;
// Epaisseur d'un trait de ligne ou d'un point
static float sEpaisseurDeTrait = 1.f;

/* Valeur de la callback de rafraichissement de fenetre */
static FonctionVide sFonctionAffichage = (FonctionVide)NULL;
/* Valeur de la callback de gestion du clavier */
static FonctionCharDeuxInts sFonctionClavier = (FonctionCharDeuxInts)NULL;
/* Valeur de la callback de gestion des touches speciales du clavier */
static FonctionTroisInts sFonctionClavierSpecial = (FonctionTroisInts)NULL;
/* Valeur de la callback de gestion des boutons souris */
static FonctionQuatreInts sFonctionBoutonsSouris = (FonctionQuatreInts)NULL;
/* Valeur de la callback de deplacement de la souris */
static FonctionDeuxInts sFonctionDeplacementSouris = (FonctionDeuxInts)NULL;
/* Valeur de la callback de deplacement passif de la souris */
static FonctionDeuxInts sFonctionDeplacementPassifSouris = (FonctionDeuxInts)NULL;
/* Valeur de la callback de redimensionnement de la fenetre */
static FonctionDeuxInts sFonctionRedimensionnement = (FonctionDeuxInts)NULL;


/* Fonction appelee par la bibliotheque pour gerer chacun des evenements generes */
extern void gestionEvenement(EvenementGfx evenement);


/* Fonction callback par defaut appelee lors d'un rafraichissement de fenetre */
static void fonctionAffichage(void);
/* Fonction callback par defaut appelee lors de l'appui sur une touche du clavier */
static void fonctionClavier(GLubyte caractere, int xSouris, int ySouris);
/* Fonction callback par defaut appelee lors de l'appui sur une touche speciale du clavier */
static void fonctionClavierSpecial(int codeTouche, int xSouris, int ySouris);
/* Fonction callback par defaut appelee lors d'un clic de bouton souris */
static void fonctionBoutonsSouris(int bouton, int etat, int xSouris, int ySouris);
/* Fonction callback par defaut appelee lors du deplacement de la souris */
static void fonctionDeplacementSouris(int xSouris, int ySouris);
/* Fonction callback par defaut appelee lors du deplacement passif de la souris */
static void fonctionDeplacementPassifSouris(int xSouris, int ySouris);
/* Fonction callback par defaut appelee lors du redimensionnement de la fenetre */
static void fonctionRedimensionnement(int largeur, int hauteur);
/* Fonction callback appelee lors d'un evenement timer */
static void fonctionTimer(int code);



// Pour initialiser la partie graphie de la bibliotheque
void initialiseGfx(int argc, char **argv)
{
#ifdef __linux__
	int versionMajeureOGL;
	int versionMineureOGL;
	Display *affichage = XOpenDisplay(NULL);
	if (affichage != NULL)
	{
		glXQueryVersion(affichage, &versionMajeureOGL, &versionMineureOGL);
		// Juste pour supprimer le bogue present dans les dernieres versions
		// qui pose probleme avec VirtualBox
		XCloseDisplay(affichage);
	}
#endif
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
}

static void prepareFenetre_En_DeTaille(const char *nom, int xCoinHautGauche, int yCoinHautGauche, int largeur, int hauteur);
/* Prepare une fenetre en specifiant son nom, sa largeur et sa hauteur */
void prepareFenetreGraphique(const char *nom, int largeur, int hauteur)
{
	static int positionDepart = 32;
	
	sLargeurFenetre = largeur;
	sHauteurFenetre = hauteur;
	prepareFenetre_En_DeTaille(nom, positionDepart, positionDepart, largeur, hauteur);
	positionDepart = ((positionDepart-32+8)&0x03F)+32;
}

/* Prepare une fenetre en specifiant son nom, la position de son coin superieur
	gauche, ainsi que sa largeur et sa hauteur */
static void prepareFenetre_En_DeTaille(const char *nom, int xCoinHautGauche, int yCoinHautGauche, int largeur, int hauteur)
{
	glutInitWindowPosition(xCoinHautGauche, yCoinHautGauche);
	glutInitWindowSize(largeur, hauteur);
	sIDWindow = glutCreateWindow(nom);

	/* Les fonctions callback doivent etre fixees apres creation de la fenetre,
	   c'est pourquoi on memorise juste les valeurs par defaut ci-apres; cela permet
	   de plus de personnaliser les callback apres coup: les callback peuvent etre
	   deroutees de leurs valeurs par defaut si besoin est, il suffit d'appeler les
	   fonctions adequates avant de lancer la boucle des evenements */
	if (sFonctionAffichage == NULL) sFonctionAffichage = fonctionAffichage;
	/* La fonction de gestion d'inactivite n'est pas geree par defaut car elle consomme trop de ressources */
	if (sFonctionClavier == NULL) sFonctionClavier = fonctionClavier;
	if (sFonctionClavierSpecial == NULL) sFonctionClavierSpecial = fonctionClavierSpecial;
	if (sFonctionBoutonsSouris == NULL) sFonctionBoutonsSouris = fonctionBoutonsSouris;
	if (sFonctionDeplacementSouris == NULL) sFonctionDeplacementSouris = fonctionDeplacementSouris;
	// La fonction de gestion de deplacement passif souris n'est pas geree par defaut car elle consomme trop de ressources
	if (sFonctionRedimensionnement == NULL) sFonctionRedimensionnement = fonctionRedimensionnement;

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); /* Correction de perspective */
	glDepthFunc(GL_LEQUAL); /* Type de test de profondeur */

	glShadeModel(GL_SMOOTH); // Rendu lisse

	/* Mise en place de l'anti-aliasing */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	
	/* Normalisation des vecteurs normaux ;-) */
	glEnable(GL_NORMALIZE);

	epaisseurDeTrait(1);

	sLargeurFenetre = largeur;
	sHauteurFenetre = hauteur;

/*	glDrawBuffer(GL_BACK);
	glReadBuffer(GL_FRONT); */
}

// Lance la boucle de gestion des evenements
void lanceBoucleEvenements(void)
{
	/* Positionne les callbacks */
	glutDisplayFunc(sFonctionAffichage);
	glutKeyboardFunc(sFonctionClavier);
	glutSpecialFunc(sFonctionClavierSpecial);
	glutMouseFunc(sFonctionBoutonsSouris);
	glutMotionFunc(sFonctionDeplacementSouris);
	glutPassiveMotionFunc(sFonctionDeplacementPassifSouris);
	glutReshapeFunc(sFonctionRedimensionnement);

	/* Indique que la fonction ci-presente a ete appelee */
	boucleEvenementsLancee = true;
	
	/* Lance le message d'initialisation de la boucle d'evenements */
	gestionEvenement(Initialisation);

	/* Lance la boucle GLUT de gestion des evenements */
	glutMainLoop();
}

/* Termine le programme proprement */
void termineBoucleEvenements(void)
{
//	glutLeaveMainLoop();
	glutDestroyWindow(sIDWindow);
	exit(0);
}

/* Demande la mise a jour du contenu graphique de la fenetre, en lancant un message d'affichage */
void rafraichisFenetre(void)
{
	glutPostRedisplay();
}

/* Efface la fenetre avec la couleur donnee
	Les composantes rouge, vert et bleu s'etendent de 0 a 255 inclus */
void effaceFenetre(int rouge, int vert, int bleu)
{
	glClearColor(rouge/255.f, vert/255.f, bleu/255.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//	glLoadIdentity(); /* Prepare l'affichage dans la fenetre */
}

/* Selectionne la couleur courante de dessin
	Les composantes rouge, vert et bleu s'etendent de 0 a 255 inclus */
void couleurCourante(int rouge, int vert, int bleu)
{
	glColor3f(rouge/255.f, vert/255.f, bleu/255.f);
}

// Définit l'épaisseur de trait EN PIXELS DE LA FENETRE servant a afficher les points et les lignes
// Renvoie la dernière épaisseur de trait utilisée
float epaisseurDeTrait(float epaisseur)
{
	const float ancienneEpaisseur = sEpaisseurDeTrait;
	sEpaisseurDeTrait = epaisseur;
	glPointSize(epaisseur);
	glLineWidth(epaisseur);
	return ancienneEpaisseur;
}

/* Dessine un point de couleur courante aux coordonnees donnees */
void point(float x, float y)
{
	glBegin(GL_POINTS);
		glVertex2f(x, y);
	glEnd();
}


/* Dessine une ligne de couleur courante aux coordonnees donnees */
void ligne(float xDepart, float yDepart, float xArrivee, float yArrivee)
{
	glBegin(GL_LINES);
		glVertex2f(xDepart, yDepart);
		glVertex2f(xArrivee, yArrivee);
	glEnd();
}


/* Dessine un triangle de couleur courante aux coordonnees donnees */
void triangle(float xCoin1, float yCoin1, float xCoin2, float yCoin2, float xCoin3, float yCoin3)
{
	glBegin(GL_TRIANGLES);
		glVertex2f(xCoin1, yCoin1);
		glVertex2f(xCoin2, yCoin2);
		glVertex2f(xCoin3, yCoin3);
	glEnd();

	// Retrace le contour pour l'adoucir
	
	if (sEpaisseurDeTrait != 1.f)
		glLineWidth(1.f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(xCoin1, yCoin1);
		glVertex2f(xCoin2, yCoin2);
		glVertex2f(xCoin3, yCoin3);
	glEnd();
	if (sEpaisseurDeTrait != 1.f)
		glLineWidth(sEpaisseurDeTrait);
}

/* Dessine un rectangle de couleur courante aux coordonnees donnees */
void rectangle(float xCoin1, float yCoin1, float xCoin2, float yCoin2)
{
//	glRectf(xCoin1, yCoin1, xCoin2, yCoin2);
	glBegin(GL_QUADS);
		glVertex2f(xCoin1, yCoin1);
		glVertex2f(xCoin2, yCoin1);
		glVertex2f(xCoin2, yCoin2);
		glVertex2f(xCoin1, yCoin2);
	glEnd();
}

/* Affiche une chaine de caracteres de taille donnee a la position specifiee */
void afficheChaine(const char *chaine, float taille, float x, float y)
{
	char *pointeurChaine = (char *)chaine;
	const float tailleScaling = taille/120.f;
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(tailleScaling, tailleScaling, tailleScaling);
	while (*pointeurChaine)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *pointeurChaine++);
	glPopMatrix();
}

/* Renvoie l'espace occupe par la chaine de caracteres de taille donnee */
float tailleChaine(const char *chaine, float taille)
{
	return glutStrokeLength(GLUT_STROKE_ROMAN, (const unsigned char *)chaine)*taille/120.f;
}

/* Envoie un message rafraichisFenetre apres la duree specifiee */
void demandeRedessinDans_ms(int millisecondes)
{
	glutTimerFunc(millisecondes, fonctionTimer, -1);
}

/* Fonction callback appelee lors d'un evenement timer */
static void fonctionTimer(int code)
{
	if (code < 0)
		rafraichisFenetre();
	else	// code >= 0
	{
		int millisecondesEntreAppels = sMillisecondesEntreAppels;
		if (millisecondesEntreAppels >= 0)	// Si le nombre de millisecondes est correct
		{
			glutTimerFunc((unsigned int)millisecondesEntreAppels, fonctionTimer, millisecondesEntreAppels);	// On relance le timer
			gestionEvenement(Temporisation);	// On envoie le message Temporisation
		}
	}
}

// Demarre l'envoi periodique de messages Temporisation
void demandeTemporisation(int millisecondesEntreAppels)
{
	if (sMillisecondesEntreAppels < 0 && millisecondesEntreAppels >= 0)	// Si le timer n'est pas active et que le nombre de millisecondes est correct
		glutTimerFunc((unsigned int)millisecondesEntreAppels, fonctionTimer, millisecondesEntreAppels);	// On lance un timer qui s'auto-entretient
	sMillisecondesEntreAppels = millisecondesEntreAppels;
}

/* Renvoie la composante rouge du pixel */
static unsigned char rougeDuPixel(int pixel)
{
	return (pixel >> 16)&0x0FF;
}

/* Renvoie la composante verte du pixel */
static unsigned char vertDuPixel(int pixel)
{
	return (pixel >> 8)&0x0FF;
}

/* Renvoie la composante bleue du pixel */
static unsigned char bleuDuPixel(int pixel)
{
	return pixel&0x0FF;
}

/* Lis une portion de la fenetre, couvrant largeur*hauteur et demarrant en (x, y)
Les donnees lues sont sauvees comme une succession de valeurs B, V, R de type
unsigned char */
void lisImage(int x, int y, int largeur, int hauteur, unsigned char *donnees) {
	unsigned char *ptrDonnees;
	int *pixels = (int*)malloc(largeur*hauteur*sizeof(int));
	int *ptrPixel;
	int index;
	glReadPixels(x, y, largeur, hauteur, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixels);
	ptrPixel = pixels;
	ptrDonnees = donnees;
	for (index = largeur*hauteur; index != 0; --index) { // On parcourt tous les pixels lus
		int pixel = *ptrPixel;					// On lit le pixel courant
		// On extrait chaque composante du pixel, que l'on met dans les donnees
		ptrDonnees[0] = bleuDuPixel(pixel);
		ptrDonnees[1] = vertDuPixel(pixel);
		ptrDonnees[2] = rougeDuPixel(pixel);
		++ptrPixel;								// On passe au pixel suivant
		ptrDonnees += 3;						// On passe aux donnees suivantes		
	}
	free(pixels);
}

// Convertit largeur*hauteur pixels de la forme bleu, vert, rouge en entiers opacite, rouge, vert, bleu little endian
static int *BVR2ARVB(int largeur, int hauteur, const unsigned char *donnees) {
	const unsigned char *ptrDonnees;
	unsigned char *pixels = (unsigned char*)malloc(largeur*hauteur*sizeof(int));
	unsigned char *ptrPixel;
	int index;
	ptrPixel = pixels;
	ptrDonnees = donnees;
	for
		(index = largeur*hauteur; index != 0; --index) /* On parcourt tous les
														pixels de l'image */
	{
		ptrPixel[0] = ptrDonnees[0];
		ptrPixel[1] = ptrDonnees[1];
		ptrPixel[2] = ptrDonnees[2];
		ptrPixel[3] = 0x0FF;
		ptrDonnees += 3; /* On passe a la premiere composante du pixel suivant */
		ptrPixel += 4; /* On passe au pixel suivant */
	}
	return (int*)pixels;
}

// Ecris dans la fenetre une image BVR couvrant largeur*hauteur et demarrant en (x, y) */
void ecrisImage(int x, int y, int largeur, int hauteur, const unsigned char *donnees)
{
	int *pixels = BVR2ARVB(largeur, hauteur, donnees);
	ecrisImageARVB(x, y, largeur, hauteur, pixels);
	free(pixels);
}

// Ecris une image couvrant largeur*hauteur, demarrant en (x, y) et faite de
// pixels ARVB (stockes dans des int en little endian) dans la fenetre */
void ecrisImageARVB(int x, int y, int largeur, int hauteur, const int *donnees)
{
	glRasterPos2i(x, y);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glDrawPixels(largeur, hauteur, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, donnees);
}

// Cree une texture reutilisable de largeur et hauteur donnees, faites de pixels ABVR little endian
Texture2D *creeTexture2D(int largeur, int hauteur, const int *pixels)
{
	Texture2D *texture = (Texture2D*)malloc(sizeof(Texture2D));
	int largeur2m = 64;
	int hauteur2m = 64;
	
	texture->largeur = largeur;
	texture->hauteur = hauteur;
	while(largeur2m < largeur) largeur2m += largeur2m; // On calcule la largeur sous la forme 2^m
	while(hauteur2m < hauteur) hauteur2m += hauteur2m; // On calcule la hauteur sous la forme 2^m
	texture->proportionX = (float)largeur/(float)largeur2m;
	texture->proportionY = (float)hauteur/(float)hauteur2m;
	
	texture->donnees = (int*)calloc(largeur2m*hauteur2m, sizeof(int));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texture->numero);
	glBindTexture(GL_TEXTURE_2D, texture->numero);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, largeur2m, hauteur2m, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->donnees);
	modifieTexture2D(texture, pixels);

	return texture;
}

// Permet de modifier les pixels de la texture prealablement definie, en donnant un nouveau tableau de pixels ABVR little endian
void modifieTexture2D(const Texture2D *texture, const int *pixelsABVR)
{
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture->largeur, texture->hauteur, GL_RGBA, GL_UNSIGNED_BYTE, pixelsABVR);
}

// Affiche la texture passee en parametre aux coordonnees fournies
void rectangleSelonTexture(float x, float y, const Texture2D *texture)
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture->numero);
	glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f);
		glVertex2f(x, y);
		glTexCoord2f(texture->proportionX, 0.f);
		glVertex2f(x+texture->largeur, y);
		glTexCoord2f(texture->proportionX, texture->proportionY);
		glVertex2f(x+texture->largeur, y+texture->hauteur);
		glTexCoord2f(0.f, texture->proportionY);
		glVertex2f(x, y+texture->hauteur);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

// Libere les ressources associees a la gestion de la texture
void libereTexture(Texture2D **texture)
{
	free((*texture)->donnees);
	glDeleteTextures(1, &(*texture)->numero);
	free(*texture);
	*texture = NULL;
}


/* Renvoie l'abscisse de souris correspondant au dernier evenement Clavier, Souris ou BoutonSouris */
int abscisseSouris(void)
{
	return sAbscisseSouris;
}

/* Renvoie l'ordonnee de souris correspondant au dernier evenement Clavier, Souris ou BoutonSouris */
int ordonneeSouris(void)
{
	return sOrdonneeSouris;
}

/* Renvoie le dernier etat de bouton souris traite par l'evenement BoutonSouris */
EtatBoutonSouris etatBoutonSouris(void)
{
	return sEtatBoutonSouris;
}

/* Renvoie le dernier caractere clavier traite par l'evenement Clavier */
char caractereClavier(void)
{
	return (char)sCaractereClavier;
}

/* Renvoie la derniere touche speciale du clavier traitee par l'evenement ClavierSpecial */
int toucheClavier(void)
{
	return sToucheClavier;
}


// Indique l'etat de la touche Shift lors du dernier evenement Clavier
bool toucheShiftAppuyee(void)
{
    return (glutGetModifiers()&GLUT_ACTIVE_SHIFT) != 0;
}

// Indique l'etat de la touche Ctrl lors du dernier evenement Clavier
bool toucheCtrlAppuyee(void)
{
    return (glutGetModifiers()&GLUT_ACTIVE_CTRL) != 0;
}

// Indique l'etat de la touche Alt lors du dernier evenement Clavier
bool toucheAltAppuyee(void)
{
    return (glutGetModifiers()&GLUT_ACTIVE_ALT) != 0;
}


// Renvoie la largeur actuelle de la fenetre
int largeurFenetre(void)
{
	return sLargeurFenetre;
}

// Renvoie la hauteur actuelle de la fenetre
int hauteurFenetre(void)
{
	return sHauteurFenetre;
}

// Demande au systeme de redimensionner la fenetre
void redimensionneFenetre(int largeur, int hauteur)
{
#ifdef _WIN32
	glutPositionWindow(32, 32);
#endif
	glutReshapeWindow(largeur, hauteur);
}

// Demande au systeme de passer la fenetre en plein ecran
void modePleinEcran(void)
{
	glutFullScreen();
}

// Affiche un message d'erreur de maniere standard pour l'OS
void messageDErreur(const char *message)
{
	fprintf(stderr, "Erreur GfxLib : %s\n", message);
}

/* Active la gestion du deplacement passif de la souris */
void activeGestionDeplacementPassifSouris(void)
{
	sFonctionDeplacementPassifSouris = fonctionDeplacementPassifSouris;
	/* Met a jour la callback si la boucle a ete lancee pour modification immediate */
	if (boucleEvenementsLancee)
		glutPassiveMotionFunc(sFonctionDeplacementPassifSouris);
}

// *****************************************************************************


/* Fonction callback par defaut appelee lors d'un rafraichissement de fenetre */
static void fonctionAffichage(void)
{
#ifdef DEBUG_MODE
	GLenum erreur;
#endif
//	glLoadIdentity();
	
	glFlush();
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	/* On appelle tout simplement la fonction generique de gestion des evenements */
	gestionEvenement(Affichage);
	glutSwapBuffers();
	
#ifdef DEBUG_MODE
	/* Afficher une erreur OpenGL si celle-ci a eu lieu */
	if
	   ((erreur = glGetError()) != GL_NO_ERROR)
	{
		printf("Erreur OpenGL %d : ", (int)erreur);
		puts((const char *)gluErrorString(erreur));
	}
#endif
}

/* Fonction callback par defaut appelee lors de l'appui sur une touche du clavier */
static void fonctionClavier(GLubyte caractere, int xSouris, int ySouris)
{
	/* On memorise le caractere et la position graphique (et pas fenetre) de la souris pour pouvoir les fournir par la suite */
	sCaractereClavier = caractere;
#ifdef ACTION_CLAVIER_MEMORISE_POSITION_SOURIS
	sAbscisseSouris = xSouris;
	sOrdonneeSouris = sHauteurFenetre-ySouris;
#endif

//	On memorise l'etat des modificateurs Shift, Ctrl et Alt
//	sAppuiToucheShift = (glutGetModifiers ()&GLUT_ACTIVE_SHIFT) != 0;
//	sAppuiToucheCtrl = (glutGetModifiers ()&GLUT_ACTIVE_CTRL) != 0;
//	sAppuiToucheAlt = (glutGetModifiers ()&GLUT_ACTIVE_ALT) != 0;

	/* Puis on appelle la fonction generique de gestion des evenements */
	gestionEvenement(Clavier);
}

/* Fonction callback par defaut appelee lors de l'appui sur une touche speciale du clavier */
static void fonctionClavierSpecial(int codeTouche, int xSouris, int ySouris)
{
	/* On memorise la touche et eventuellement la position graphique (et pas fenetre) de la souris pour pouvoir les fournir par la suite */
	switch
		(codeTouche)
	{
		case GLUT_KEY_F1:
			sToucheClavier = ToucheF1;
			break;
			
		case GLUT_KEY_F2:
			sToucheClavier = ToucheF2;
			break;
			
		case GLUT_KEY_F3:
			sToucheClavier = ToucheF3;
			break;
			
		case GLUT_KEY_F4:
			sToucheClavier = ToucheF4;
			break;
			
		case GLUT_KEY_F5:
			sToucheClavier = ToucheF5;
			break;
			
		case GLUT_KEY_F6:
			sToucheClavier = ToucheF6;
			break;
			
		case GLUT_KEY_F7:
			sToucheClavier = ToucheF7;
			break;
			
		case GLUT_KEY_F8:
			sToucheClavier = ToucheF8;
			break;
			
		case GLUT_KEY_F9:
			sToucheClavier = ToucheF9;
			break;
			
		case GLUT_KEY_F10:
			sToucheClavier = ToucheF10;
			break;
			
		case GLUT_KEY_F11:
			sToucheClavier = ToucheF11;
			break;
			
		case GLUT_KEY_F12:
			sToucheClavier = ToucheF12;
			break;
			
		case GLUT_KEY_UP:
			sToucheClavier = ToucheFlecheHaut;
			break;
			
		case GLUT_KEY_DOWN:
			sToucheClavier = ToucheFlecheBas;
			break;
			
		case GLUT_KEY_LEFT:
			sToucheClavier = ToucheFlecheGauche;
			break;
			
		case GLUT_KEY_RIGHT:
			sToucheClavier = ToucheFlecheDroite;
			break;
			
		default:
			sToucheClavier = ToucheNonGeree;
	}
	
#ifdef ACTION_CLAVIER_MEMORISE_POSITION_SOURIS
	sAbscisseSouris = xSouris;
	sOrdonneeSouris = sHauteurFenetre-ySouris;
#endif

//	On memorise l'etat des modificateurs Shift, Ctrl et Alt
//	sAppuiToucheShift = (glutGetModifiers ()&GLUT_ACTIVE_SHIFT) != 0;
//	sAppuiToucheCtrl = (glutGetModifiers ()&GLUT_ACTIVE_CTRL) != 0;
//	sAppuiToucheAlt = (glutGetModifiers ()&GLUT_ACTIVE_ALT) != 0;

	/* Puis on appelle la fonction generique de gestion des evenements */
	gestionEvenement(ClavierSpecial);
}

/* Fonction callback par defaut appelee lors d'un clic de bouton souris */
static void fonctionBoutonsSouris(int bouton, int etat, int xSouris, int ySouris)
{
	switch (bouton)
	{
		case GLUT_LEFT_BUTTON:
			sEtatBoutonSouris = (etat == GLUT_DOWN ? GaucheAppuye : GaucheRelache);
			break;
		case GLUT_RIGHT_BUTTON:
			sEtatBoutonSouris = (etat == GLUT_DOWN ? DroiteAppuye : DroiteRelache);
			break;
		case GLUT_MIDDLE_BUTTON:
			sEtatBoutonSouris = (etat == GLUT_DOWN ? MilieuAppuye : MilieuRelache);
			break;
		case 3:
			sEtatBoutonSouris = ScrollDown;
			break;
		case 4:
			sEtatBoutonSouris = ScrollUp;
			break;
		case 5:
			sEtatBoutonSouris = ScrollRight;
			break;
		case 6:
			sEtatBoutonSouris = ScrollLeft;
			break;
		default:
			sEtatBoutonSouris = bouton;
	}

//	On memorise l'etat des modificateurs Shift, Ctrl et Alt
//	sAppuiToucheShift = (glutGetModifiers ()&GLUT_ACTIVE_SHIFT) != 0;
//	sAppuiToucheCtrl = (glutGetModifiers ()&GLUT_ACTIVE_CTRL) != 0;
//	sAppuiToucheAlt = (glutGetModifiers ()&GLUT_ACTIVE_ALT) != 0;
// 	On memorise les coordonnees graphiques (et pas fenetre) de la souris au moment de l'evenement
	sAbscisseSouris = xSouris;
	sOrdonneeSouris = sHauteurFenetre-ySouris;

// 	Puis on appelle la fonction generique de gestion des evenements
	gestionEvenement(BoutonSouris);
}

/* Fonction callback par defaut appelee lors du deplacement de la souris */
static void fonctionDeplacementSouris(int xSouris, int ySouris)
{
	/* On memorise les coordonnees graphiques (et pas fenetre) de la souris au moment de l'evenement */
	sAbscisseSouris = xSouris;
	sOrdonneeSouris = sHauteurFenetre-ySouris;
	/* Puis on appelle la fonction generique de gestion des evenements */
	gestionEvenement(Souris);
}

/* Fonction callback par defaut appelee lors du deplacement de la souris */
static void fonctionDeplacementPassifSouris(int xSouris, int ySouris)
{
	/* On memorise les coordonnees graphiques (et pas fenetre) de la souris au moment de l'evenement */
	sAbscisseSouris = xSouris;
	sOrdonneeSouris = sHauteurFenetre-ySouris;
	/* Puis on appelle la fonction generique de gestion des evenements */
	gestionEvenement(Souris);
}

/* Fonction callback par defaut appelee lors du redimensionnement d'une fenetre */
static void fonctionRedimensionnement(int largeur, int hauteur)
{
	glViewport(0, 0, largeur, hauteur);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/* Prepare la fenetre pour une utilisation en 2D */
	glOrtho(0., (GLdouble)largeur, 0., (GLdouble)hauteur, -1., 1.);
	glMatrixMode (GL_MODELVIEW);
	sLargeurFenetre = largeur;
	sHauteurFenetre = hauteur;
	gestionEvenement(Redimensionnement);
}

// Veuillez reporter tout commentaire a ghislain.oudinet@yncrea.fr


// 	Version 1.3 (21/06/2022) : ajout de quelques messages d'erreur sur stderr dans la lecture du BMP
// 	Version 1.2 (21/01/2009) : ajout du support en lecture des BMP orientes de haut en bas
// 	Version 1.1 : modification du code de libereDonneesImageRGB pour le rendre
// 		plus robuste.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/BmpLib.h"
#include "../include/OutilsLib.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

//#define DEBUGMODE

/* Fonction s'occupant de la liberation complete d'une structure DonneesImageRGB */
void libereDonneesImageRGB(DonneesImageRGB **structure)
{
	if (structure != NULL)
	{
		if (*structure != NULL)
		{
			free((*structure)->donneesRGB);
			free(*structure);
		}
		*structure = NULL;
	}
}

/* Renvoie la longueur d'une scanline, qui doit etre multiple de quatre octets */
static int tailleScanLineRGB(int largeurImage)
{
	return (largeurImage*3+3)&(int)0x0FFFFFFFCL;
}

/* Fonction essayant de lire le fichier passe en parametre, et renvoyant une structure
	contenant les informations de l'image en cas de succes, NULL sinon */
DonneesImageRGB *lisBMPRGB(char *nom)
{
	DonneesImageRGB *donneesImage = (DonneesImageRGB*)calloc(1, sizeof(DonneesImageRGB));
	bool toutOK = false;
	
	if (donneesImage != NULL)
	{
// 		donneesImage != NULL
		FILE *fichierBMP = fopen(nom, "rb");
		if (fichierBMP != NULL)
		{
// 			fichierBMP != NULL
// 			Si le header commence bien par 'B' et 'M'...
			if (fgetc(fichierBMP) == 'B' && fgetc(fichierBMP) == 'M')
			{
// 				... alors la suite du fichier peut etre interpretee
				int fileHeader[3];
				if (fread(fileHeader, sizeof(fileHeader), 1, fichierBMP) == 1)
				{
// 					Lecture du header de fichier
					int offsetDonnees = little32VersNatif(fileHeader[2]);
					
					int bitmapInfoHeader[10];
					if (fread(bitmapInfoHeader, sizeof(bitmapInfoHeader), 1, fichierBMP) == 1)
					{
// 						On ne lit que les images 24 bits non compressees
						if ((little32VersNatif(bitmapInfoHeader[3]) == 0x00180001) && (little32VersNatif(bitmapInfoHeader[4]) == 0))
						{
							bool hautVersBas = false; // Utile pour detecter les BMP allant de haut en bas au lieu de bas vers haut
// 							Lecture du header de bitmap (uniquement les informations qui nous interessent)
							donneesImage->largeurImage = little32VersNatif(bitmapInfoHeader[1]);
							donneesImage->hauteurImage = little32VersNatif(bitmapInfoHeader[2]);

							if (donneesImage->hauteurImage < 0)
							{
								donneesImage->hauteurImage = -donneesImage->hauteurImage;
								hautVersBas = true;
							}
							
// 							On alloue la place pour lire les donnes
							if ((donneesImage->donneesRGB = (unsigned char *)malloc((unsigned int)donneesImage->largeurImage*(unsigned int)donneesImage->hauteurImage*3)) != NULL)
							{
// 								donneesImage->donneesRGB != NULL
								
								const int tailleScanLine = tailleScanLineRGB(donneesImage->largeurImage);
								unsigned char *scanline = (unsigned char *)malloc((unsigned int)tailleScanLine);
// 								Une scanline doit avoir une taille multiple de quatre octets
// 								On alloue la place pour lire chaque scanline
								if (scanline != NULL)
								{
// 									scanline != NULL
// 									On se positionne sur le debut des donnees a lire
									if (fseek(fichierBMP, offsetDonnees, SEEK_SET) == 0)
									{
										unsigned char *pointeurDonnees  = donneesImage->donneesRGB +
															(hautVersBas ?
															donneesImage->largeurImage*3*(donneesImage->hauteurImage-1) :
															0);
// 										Tout s'est bien passe jusqu'a present
										toutOK = true;
										for (int indexLigne = 0; (indexLigne < donneesImage->hauteurImage) && toutOK; ++indexLigne)
										{
// 											On recopie ligne a ligne les informations
											if (fread(scanline, (unsigned int)tailleScanLine, 1, fichierBMP) == 1)
											{
												memcpy(pointeurDonnees, scanline, (unsigned int)(donneesImage->largeurImage*3));
												if (hautVersBas)
													pointeurDonnees -= donneesImage->largeurImage*3;
												else
													pointeurDonnees += donneesImage->largeurImage*3;
											}
											else
											{
												toutOK = false;
											}
										}
									}
// 									Quoi qu'il arrive il faut liberer le buffer memoire de lecture d'une scanline
									free(scanline);
								}
// 								Si tout ne s'est pas bien passe, il faut liberer l'espace memoire pour stocker l'image
								if (!toutOK)
								{
									free(donneesImage->donneesRGB);
								}
							}
							else
							{
								fprintf(stderr, "Impossible d'allouer l'espace mémoire pour les données de l'image\n"); fflush(stderr);
							}
						}
						else
						{
							fprintf(stderr, "Le fichier n'est pas au format 24b non-compressé\n"); fflush(stderr);
						}
					}
				}
			}
			else
			{
				fprintf(stderr, "Le fichier n'est pas dans un format BMP correct\n"); fflush(stderr);
			}
// 			Quoi qu'il arrive il faut fermer le fichier
			fclose(fichierBMP);
		}
		else
		{
			fprintf(stderr, "Impossible d'ouvrir le fichier BMP en lecture\n"); fflush(stderr);
		}
		
// 		Si tout ne s'est pas bien passe on libere les donnees image et on les met a NULL
		if (!toutOK)
		{
			free(donneesImage);
			donneesImage = NULL;
		}
	}
	else
	{
		fprintf(stderr, "Pas assez de mémoire pour stocker l'image\n"); fflush(stderr);
	}
	
	return donneesImage;
}

/* Fonction ecrivant les informations de l'image dans le fichier passe en parametre.
	Renvoie faux en cas de probleme, vrai sinon */
bool ecrisBMPRGB_Dans(DonneesImageRGB *donneesImage, char *nom)
{
	FILE *fichierBMP;
	bool toutOK = false;
	
	if ((fichierBMP = fopen(nom, "wb")) != NULL)
	{
// 		fichierBMP != NULL
// 		Le header commence par 'B' et 'M'
		if (fwrite("BM", 2, 1, fichierBMP) == 1)
		{
			int headerBMP[14]; /* Sert a memoriser toutes les informations relatives au BMP */
			const int tailleScanLine = tailleScanLineRGB(donneesImage->largeurImage);
			
			memset(headerBMP, 0, 14*4);
			/* Taille du fichier */
			headerBMP[0] = natif32VersLittle(2+12+40+4+tailleScanLine*donneesImage->hauteurImage);
			headerBMP[1] = 0;
			/* Offset a partir duquel commencent les donnees */
			headerBMP[2] = natif32VersLittle(2+12+40+4);
			/* Taille de la structure BITMAPINFOHEADER qui doit faire 40 octets */
			headerBMP[3] = natif32VersLittle(40);
			/* Largeur de l'image */
			headerBMP[4] = natif32VersLittle(donneesImage->largeurImage);
			/* Hauteur de l'image */
			headerBMP[5] = natif32VersLittle(donneesImage->hauteurImage);
			/* Image monoplan de 24 bits */
			headerBMP[6] = natif32VersLittle(0x00180001);
			/* Image non compressee */
			headerBMP[7] = 0;
			/* Les six champs suivants ne nous sont pas utiles */
			headerBMP[8] = 0;
			headerBMP[9] = 0;
			headerBMP[10] = 0;
			headerBMP[11] = 0;
			headerBMP[12] = 0;
			headerBMP[13] = 0;
			
			/* Ecriture du header de fichier */
			if (fwrite(headerBMP, 14*4, 1, fichierBMP) == 1)
			{
				unsigned char *scanline= (unsigned char *)calloc(1, (unsigned int)tailleScanLine);
				/* On alloue la place pour ecrire chaque scanline */
				if (scanline != NULL)
				{
					/* scanline != NULL */
					unsigned char *pointeurDonnees = donneesImage->donneesRGB;
					
					/* Tout s'est bien passe jusqu'a present */
					toutOK = true;
					
					for (int indexLigne = 0; (indexLigne < donneesImage->hauteurImage) && toutOK; ++indexLigne)
					{
						/* On recopie ligne a ligne les informations */
						memcpy(scanline, pointeurDonnees, (unsigned int)(donneesImage->largeurImage*3));
						pointeurDonnees += donneesImage->largeurImage*3;
						
						if (fwrite(scanline, (unsigned int)tailleScanLine, 1, fichierBMP) != 1)
						{
							toutOK = false;
						}
					}

					/* Quoi qu'il arrive il faut liberer le buffer memoire d'ecriture d'une scanline */
					free(scanline);
				}
			}
		}
		
		
		/* Quoi qu'il arrive il faut fermer le fichier */
		fclose(fichierBMP);
	}
	else
	{
		fprintf(stderr, "Impossible d'ouvrir le fichier BMP en écriture\n"); fflush(stderr);
	}
	
	return toutOK;
}

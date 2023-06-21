//a mettre en variable globale
DonneesImageRGB *image = NULL;
int *pixels = NULL;
Texture2D *texture = NULL;

//a intégrer quelque part
static int *BVR2ARVB(int largeur, int hauteur, const unsigned char *donnees)
{
	const unsigned char *ptrDonnees;
	unsigned char *pixels = (unsigned char *)malloc(largeur * hauteur * 4 * sizeof(unsigned char));
	unsigned char *ptrPixel;
	int index;
	ptrPixel = pixels;
	ptrDonnees = donnees;
	for (index = largeur * hauteur; index != 0; --index)
	{
		/* On parcourt tous les pixels de l'image */
		if (ptrDonnees[0] == 0xFF && ptrDonnees[1] == 0x00 && ptrDonnees[2] == 0xFF)
		{
			ptrPixel[0] = ptrDonnees[0];
			ptrPixel[1] = ptrDonnees[1];
			ptrPixel[2] = ptrDonnees[2];
			ptrPixel[3] = 0x00; // Opacité à 0
		}
		else
		{
			ptrPixel[0] = ptrDonnees[0];
			ptrPixel[1] = ptrDonnees[1];
			ptrPixel[2] = ptrDonnees[2];
			ptrPixel[3] = 0xFF; // Opacité à 255
		}
		ptrDonnees += 3; /* On passe à la première composante du pixel suivant */
		ptrPixel += 4;	 /* On passe au pixel suivant */
	}
	return (int *)pixels;
}

//a intégrer dans le même fichier, INDISPENSABLE pour la transparence
void supprimerCouleur(Texture2D *texture, int couleurR, int couleurV, int couleurB)
{
	int *pixels = texture->donnees;
	int largeur = texture->largeur;
	int hauteur = texture->hauteur;

	for (int i = 0; i < largeur * hauteur; i++)
	{
		// Extraire les composantes RGBA du pixel
		int alpha = (pixels[i] >> 24) & 0xFF;
		int rouge = (pixels[i] >> 16) & 0xFF;
		int vert = (pixels[i] >> 8) & 0xFF;
		int bleu = pixels[i] & 0xFF;

		// Vérifier si le pixel correspond à la couleur à supprimer (R, V, B)
		if (rouge == couleurR && vert == couleurV && bleu == couleurB)
		{
			// Mettre l'opacité à 0
			alpha = 0;
		}

		// Reconstruire le pixel modifié
		pixels[i] = (alpha << 24) | (rouge << 16) | (vert << 8) | bleu;
	}
	// Mettre à jour la texture avec les nouveaux pixels
	modifieTexture2D(texture, pixels);
}

//EXEMPLE DE SYNTAXE POUR FAIRE FONCTIONNER
void gestionEvenement(EvenementGfx evenement)
{
	static bool pleinEcran = false;		  // Pour savoir si on est en mode plein ecran ou pas
	static DonneesImageRGB *image = NULL; // L'image a afficher au centre de l'ecran

	switch (evenement)
	{
	case Initialisation:
   
		image = lisBMPRGB("IMG/fusil_front.bmp");//Lis l'image
		pixels = BVR2ARVB(image->largeurImage, image->hauteurImage, image->donneesRGB);//La transforme en ARVB
		texture = creeTexture2D(image->largeurImage, image->hauteurImage, pixels);//Créer la texture
      
		demandeTemporisation(20);
		break;

	case Temporisation:
		rafraichisFenetre();
		break;

	case Affichage:
		effaceFenetre(255, 255, 255);

		if (image != NULL) // Si l'image a pu être lue
		{
			// Afficher la texture avec la fonction rectangleSelonTexture ET GÈRE LA POSITION
			rectangleSelonTexture(largeurFenetre() / 2, hauteurFenetre() / 2, texture);
		}
		break;
}

/*
 *  VectorLib.c
 *  isentlib
 *
 *  Created by Ghislain Oudinet on 27/09/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "../include/VectorLib.h"

#ifdef __SSE2__



// Calcule la somme membre a membre de deux tableaux de Packed Single SSE (= 4 floats) dans un troisieme tableau
// La taille est exprimee en nombre de vecteurs (et pas en nombre de floats)
// Les tableaux doivent etre alignes sur des frontieres de 16 octets
void sommeVecteur_Et_Dans_DeTaille(__m128 *source1, __m128 *source2, __m128 *destination, int taille)
{
	int compteur;
	
	for (compteur = 0; compteur < taille; ++compteur)
		*destination++ = _mm_add_ps(*source1++, *source2++);
}

// Calcule la difference membre a membre de deux tableaux de Packed Single SSE (= 4 floats) dans un troisieme tableau
// La taille est exprimee en nombre de vecteurs (et pas en nombre de floats)
// Les tableaux doivent etre alignes sur des frontieres de 16 octets
void vecteur_Moins_Dans_DeTaille(__m128 *source1, __m128 *source2, __m128 *destination, int taille)
{
	int compteur;
	
	for (compteur = 0; compteur < taille; ++compteur)
		*destination++ = _mm_sub_ps(*source1++, *source2++);
}

// Calcule le produit membre a membre de deux tableaux de Packed Single SSE (= 4 floats) dans un troisieme tableau
// La taille est exprimee en nombre de vecteurs (et pas en nombre de floats)
// Les tableaux doivent etre alignes sur des frontieres de 16 octets
void produitVecteur_Par_Dans_DeTaille(__m128 *source1, __m128 *source2, __m128 *destination, int taille)
{
	int compteur;
	
	for (compteur = 0; compteur < taille; ++compteur)
		*destination++ = _mm_mul_ps(*source1++, *source2++);
}

// Calcule le minimum membre a membre de deux tableaux de Packed Single SSE (= 4 floats) dans un troisieme tableau
// La taille est exprimee en nombre de vecteurs (et pas en nombre de floats)
// Les tableaux doivent etre alignes sur des frontieres de 16 octets
void minimumVecteur_Et_Dans_DeTaille(__m128 *source1, __m128 *source2, __m128 *destination, int taille)
{
	int compteur;
	
	for (compteur = 0; compteur < taille; ++compteur)
		*destination++ = _mm_min_ps(*source1++, *source2++);
}

// Calcule le maximum membre a membre de deux tableaux de Packed Single SSE (= 4 floats) dans un troisieme tableau
// La taille est exprimee en nombre de vecteurs (et pas en nombre de floats)
// Les tableaux doivent etre alignes sur des frontieres de 16 octets
void maximumVecteur_Et_Dans_DeTaille(__m128 *source1, __m128 *source2, __m128 *destination, int taille)
{
	int compteur;
	
	for (compteur = 0; compteur < taille; ++compteur)
		*destination++ = _mm_max_ps(*source1++, *source2++);
}
#else
unsigned char NoVectorIfNoSSE = 0x0FF;
#endif

/* Veuillez reporter tout commentaire a ghislain.oudinet@isen.fr */

/*
 *  VectorLib.h
 *  isentlib
 *
 *  Created by Ghislain Oudinet on 27/09/07.
 *
 */

// Sous Linux/MacOSX : compiler le source faisant appel a la VectorLib au minimum avec l'option -msse2
// Sous DevCPP (gcc/MINGW sous Windows) : activer l'option Pentium4 (bug du compilateur/environnement ???)

#ifndef VECTORLIB_H
#define VECTORLIB_H

#ifdef __SSE2__

#include <emmintrin.h>

// Calcule la somme membre a membre de deux tableaux de Packed Single SSE (= 4 floats) dans un troisieme tableau
// La taille est exprimee en nombre de vecteurs (et pas en nombre de floats)
// Les tableaux doivent etre alignes sur des frontieres de 16 octets
void sommeVecteur_Et_Dans_DeTaille(__m128 *source1, __m128 *source2, __m128 *destination, int taille);

// Calcule la difference membre a membre de deux tableaux de Packed Single SSE (= 4 floats) dans un troisieme tableau
// La taille est exprimee en nombre de vecteurs (et pas en nombre de floats)
// Les tableaux doivent etre alignes sur des frontieres de 16 octets
void vecteur_Moins_Dans_DeTaille(__m128 *source1, __m128 *source2, __m128 *destination, int taille);

// Calcule le produit membre a membre de deux tableaux de Packed Single SSE (= 4 floats) dans un troisieme tableau
// La taille est exprimee en nombre de vecteurs (et pas en nombre de floats)
// Les tableaux doivent etre alignes sur des frontieres de 16 octets
void produitVecteur_Par_Dans_DeTaille(__m128 *source1, __m128 *source2, __m128 *destination, int taille);

// Calcule le minimum membre a membre de deux tableaux de Packed Single SSE (= 4 floats) dans un troisieme tableau
// La taille est exprimee en nombre de vecteurs (et pas en nombre de floats)
// Les tableaux doivent etre alignes sur des frontieres de 16 octets
void minimumVecteur_Et_Dans_DeTaille(__m128 *source1, __m128 *source2, __m128 *destination, int taille);

// Calcule le maximum membre a membre de deux tableaux de Packed Single SSE (= 4 floats) dans un troisieme tableau
// La taille est exprimee en nombre de vecteurs (et pas en nombre de floats)
// Les tableaux doivent etre alignes sur des frontieres de 16 octets
void maximumVecteur_Et_Dans_DeTaille(__m128 *source1, __m128 *source2, __m128 *destination, int taille);
#endif

#endif

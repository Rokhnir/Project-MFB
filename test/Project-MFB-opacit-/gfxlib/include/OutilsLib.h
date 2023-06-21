// Veuillez reporter tout commentaire a ghislain.oudinet@isen.fr

#ifndef OUTILSLIB_H
#define OUTILSLIB_H

// Fonction transformant un entier 32 bits little endian en entier natif
int little32VersNatif(int little);

// Fonction transformant un entier 32 bits natif en entier little endian
int natif32VersLittle(int natif);

// Fonction transformant un entier 16 bits little endian en entier natif
short little16VersNatif(short little);

// Fonction transformant un entier 16 bits natif en entier little endian
short natif16VersLittle(short natif);

// Fonction transformant un entier 32 bits big endian en entier natif
int big32VersNatif(int big);

// Fonction transformant un entier 32 bits natif en entier big endian
int natif32VersBig(int natif);

// Fonction transformant un entier 16 bits big endian en entier natif
short big16VersNatif(short big);

// Fonction transformant un entier 16 bits natif en entier big endian
short natif16VersBig(short natif);

#endif

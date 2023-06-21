#include "../include/OutilsLib.h"
#include <stdbool.h>

// Cette fonction teste si l'architecture est big endian
static bool systemeBigEndian()
{
	const short testArchitectureBig = 0x000F;
	return (*((char*)&testArchitectureBig) == 0x00);
}

// Cette fonction inverse l'ordre des octets d'un int 32 bits
static int barriereNatif32(int entier)
{
	int resultat;

	resultat = (int)((unsigned int)entier>>24);
	resultat |= (entier>>8)&0x0FF00;
	resultat |= (entier<<8)&0x0FF0000;
	resultat |= (entier<<24);
	
	return resultat;
}

// Cette fonction inverse l'ordre des octets d'un int 16 bits
static short barriereNatif16(short entier)
{
	return (short)((unsigned short)entier>>8) | (entier<<8);
}

// Fonction transformant un entier 32 bits little endian en entier natif
int little32VersNatif(int little)
{
	if
		(systemeBigEndian())
	{
		return barriereNatif32(little);
	}
	else
	{
		return little;
	}
}

// Fonction transformant un entier 32 bits natif en entier little endian
int natif32VersLittle(int natif)
{
	if
		(systemeBigEndian())
	{
			return barriereNatif32(natif);
	}
	else
	{
		return natif;
	}
}

// Fonction transformant un entier 16 bits little endian en entier natif
short little16VersNatif(short little)
{
	if
		(systemeBigEndian())
	{
		return barriereNatif16(little);
	}
	else
	{
		return little;
	}
}


// Fonction transformant un entier 16 bits natif en entier little endian
short natif16VersLittle(short natif)
{
	if
		(systemeBigEndian())
	{
		return barriereNatif16(natif);
	}
	else
	{
		return natif;
	}
}

// Fonction transformant un entier 32 bits big endian en entier natif
int big32VersNatif(int big)
{
	if
		(systemeBigEndian())
	{
		return big;
	}
	else
	{
		return barriereNatif32(big);
	}
}

// Fonction transformant un entier 32 bits natif en entier big endian
int natif32VersBig(int natif)
{
	if
		(systemeBigEndian())
	{
		return natif;
	}
	else
	{
		return barriereNatif32(natif);
	}
}

// Fonction transformant un entier 16 bits big endian en entier natif
short big16VersNatif(short big)
{
	if
		(systemeBigEndian())
	{
		return big;
	}
	else
	{
		return barriereNatif16(big);
	}
}


// Fonction transformant un entier 16 bits natif en entier big endian
short natif16VersBig(short natif)
{
	if
		(systemeBigEndian())
	{
		return natif;
	}
	else
	{
		return barriereNatif32(natif);
	}
}

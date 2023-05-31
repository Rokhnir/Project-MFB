/* Veuillez reporter tout commentaire a ghislain.oudinet@isen.fr */

/*
 *  ThreadLib.h
 *  isentlib
 *
 *  Created by Ghislain Oudinet on 26/04/07.
 *
 */

// Sous Linux : linker avec -lpthread

#ifndef THREADLIB_H
#define THREADLIB_H

#include <stdbool.h>

// Cree et lance un nouveau thread en lancant fonction sur data
// Renvoie faux si la creation du thread a echoue
bool detacheThread_sur(void*(*fonction)(void *data), void *data);



typedef void *ptrVerrou; // Le type de verrou que nous allons utiliser
// Cree un verrou
ptrVerrou creeVerrou(void);
// Ferme le verrou (s'approprie le verrou)
// Renvoie faux si la demande n'a pas ete satisfaite
bool fermeVerrou(ptrVerrou pVerrou);
// Ouvre le verrou (libere le verrou)
// Renvoie faux si l'ouverture n'a pu se faire
bool ouvreVerrou(ptrVerrou pVerrou);
// Detruit le verrou
void detruitVerrou(ptrVerrou pVerrou);

#ifndef _WIN32
// La bibliotheque propose les verrous conditionnels,  sous systeme de type POSIX pour gerer le parallelisme
typedef struct VerrouConditionnel *ptrVerrouConditionnel; // Le type de verrou conditionnel que nous allons utiliser
// Cree un verrou conditionnel
ptrVerrouConditionnel creeVerrouConditionnel(void);
// Ferme le verrou conditionnel (s'approprie le verrou)
// Renvoie faux si la demande n'a pas ete satisfaite
bool fermeVerrouConditionnel(ptrVerrouConditionnel pVerrouConditionnel);
// Attente sur le verrou conditionnel
// Renvoie faux si la demande n'a pas ete satisfaite
bool attendVerrouConditionnel(ptrVerrouConditionnel pVerrouConditionnel);
// Envoie un signal sur le verrou conditionnel
// Renvoie faux si la demande n'a pas ete satisfaite
bool signalSurVerrouConditionnel(ptrVerrouConditionnel pVerrouConditionnel);
// Envoie un signal a tous les threads en attente sur le verrou conditionnel
// Renvoie faux si la demande n'a pas ete satisfaite
bool signalUniverselSurVerrouConditionnel(ptrVerrouConditionnel pVerrouConditionnel);
// Ouvre le verrou conditionnel (libere le verrou)
// Renvoie faux si l'ouverture n'a pu se faire
bool ouvreVerrouConditionnel(ptrVerrouConditionnel pVerrouConditionnel);
// Detruit le verrou conditionnel
void detruitVerrouConditionnel(ptrVerrouConditionnel pVerrouConditionnel);
#endif

#endif

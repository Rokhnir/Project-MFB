/*
 *  ThreadLib.c
 *  isentlib
 *
 *  Created by ghis on 26/04/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "../include/ThreadLib.h"

#ifndef _WIN32

// *************************************
// Partie pthread du fichier de definition
// *************************************

#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

// Cree et lance un nouveau thread en lancant fonction sur data
// Renvoie faux si la creation du thread a echoue
bool detacheThread_sur(void*(*fonction)(void *data), void *data)
{
	pthread_t threadID;
	if (0 == pthread_create(&threadID, NULL, fonction, data))
	{
		pthread_detach(threadID);
		return true;
	}
	return false;
}

// ***** Verrou a partir d'un mutex *****

// Cree un verrou
ptrVerrou creeVerrou(void)
{
	pthread_mutex_t *mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	if (0 == pthread_mutex_init(mutex, NULL))
	{
		return mutex;
	}
	free(mutex);
	return NULL;
}

// Ferme le verrou
// Renvoie faux si la demande n'a pas ete satisfaite
bool fermeVerrou(ptrVerrou pVerrou)
{
	return 0 == pthread_mutex_lock((pthread_mutex_t *)pVerrou);
}

// Ouvre le verrou
// Renvoie faux si l'ouverture n'a pu se faire
bool ouvreVerrou(ptrVerrou pVerrou)
{
	return 0 == pthread_mutex_unlock((pthread_mutex_t *)pVerrou);
}

// Detruit le verrou
void detruitVerrou(ptrVerrou pVerrou)
{
	while (EBUSY == pthread_mutex_destroy((pthread_mutex_t *)pVerrou)) // Si le mutex est occupe on recommence
	{
		fermeVerrou(pVerrou);
		ouvreVerrou(pVerrou);
	}
	free(pVerrou); // On libere la place occupee par le mutex
}


// ***** Verrou conditionnel a partir d'une variable conditionnelle POSIX
// La bibliotheque propose les verrous conditionnels,  sous systeme de type POSIX pour gerer le parallelisme
struct VerrouConditionnel
{
	pthread_mutex_t mutex;
	pthread_cond_t variableConditionnelle;
}; // Le type de verrou conditionnel que nous allons utiliser

// Cree un verrou conditionnel
ptrVerrouConditionnel creeVerrouConditionnel(void)
{
	ptrVerrouConditionnel verrou = (ptrVerrouConditionnel)malloc(sizeof(struct VerrouConditionnel));

	if (0 == pthread_mutex_init(&verrou->mutex, NULL))
	{
		if (0 == pthread_cond_init(&verrou->variableConditionnelle, NULL))
		{
			return verrou;
		}
		pthread_mutex_destroy(&verrou->mutex);
	}
	free(verrou);
	return NULL;
}

// Ferme le verrou conditionnel (s'approprie le verrou)
// Renvoie faux si la demande n'a pas ete satisfaite
bool fermeVerrouConditionnel(ptrVerrouConditionnel pVerrouConditionnel)
{
	return 0 == pthread_mutex_lock(&pVerrouConditionnel->mutex);
}

// Attente sur le verrou conditionnel
// Renvoie faux si la demande n'a pas ete satisfaite
bool attendVerrouConditionnel(ptrVerrouConditionnel pVerrouConditionnel)
{
	return 0 == pthread_cond_wait(&pVerrouConditionnel->variableConditionnelle, &pVerrouConditionnel->mutex);
}

// Envoie un signal sur le verrou conditionnel
// Renvoie faux si la demande n'a pas ete satisfaite
bool signalSurVerrouConditionnel(ptrVerrouConditionnel pVerrouConditionnel)
{
	return 0 == pthread_cond_signal(&pVerrouConditionnel->variableConditionnelle);
}

// Envoie un signal a tous les threads en attente sur le verrou conditionnel
// Renvoie faux si la demande n'a pas ete satisfaite
bool signalUniverselSurVerrouConditionnel(ptrVerrouConditionnel pVerrouConditionnel)
{
	return 0 == pthread_cond_broadcast(&pVerrouConditionnel->variableConditionnelle);
}

// Ouvre le verrou conditionnel (libere le verrou)
// Renvoie faux si l'ouverture n'a pu se faire
bool ouvreVerrouConditionnel(ptrVerrouConditionnel pVerrouConditionnel)
{
	return 0 == pthread_mutex_unlock(&pVerrouConditionnel->mutex);
}

// Detruit le verrou conditionnel
void detruitVerrouConditionnel(ptrVerrouConditionnel pVerrouConditionnel)
{
	fermeVerrouConditionnel(pVerrouConditionnel);
	while (EBUSY == pthread_cond_destroy(&pVerrouConditionnel->variableConditionnelle)) // Si la variable conditionnelle est occupee on recommence
	{}
	ouvreVerrouConditionnel(pVerrouConditionnel);
	while (EBUSY == pthread_mutex_destroy(&pVerrouConditionnel->mutex)) // Si le mutex est occupe on recommence
	{
		fermeVerrouConditionnel(pVerrouConditionnel);
		ouvreVerrouConditionnel(pVerrouConditionnel);
	}
	free(pVerrouConditionnel); // On libere la place occupee par le mutex
}

#else

// ***************************************
// Partie Windows du fichier de definition
// ***************************************

#include <windows.h>

// Cree et lance un nouveau thread en lancant fonction sur data
// Renvoie faux si la creation du thread a echoue
bool detacheThread_sur(void*(*fonction)(void *data), void *data)
{
	HANDLE threadHandle;
	threadHandle = CreateThread (NULL, 0UL, (LPTHREAD_START_ROUTINE)fonction, data, 0, NULL);
	if (threadHandle != NULL)
    {
       CloseHandle(threadHandle);
       return true;
    }
    return false;
}

// ***** Les verrous sont crees a partir de sections critiques, plus rapides que les mutex sous Windows *****
// Cree un verrou
ptrVerrou creeVerrou(void)
{
    CRITICAL_SECTION *sectionCritique = (CRITICAL_SECTION*)malloc(sizeof(CRITICAL_SECTION));
	if (sectionCritique != NULL)
		InitializeCriticalSection(sectionCritique);
	return (ptrVerrou)sectionCritique;
}

// Ferme le verrou
// Renvoie faux si la demande n'a pas ete satisfaite
bool fermeVerrou(ptrVerrou pVerrou)
{
	EnterCriticalSection((CRITICAL_SECTION*)pVerrou);
	return true;
}

// Ouvre le verrou
// Renvoie faux si l'ouverture n'a pu se faire
bool ouvreVerrou(ptrVerrou pVerrou)
{
	LeaveCriticalSection((CRITICAL_SECTION*)pVerrou);
	return true;
}

// Detruit le verrou
void detruitVerrou(ptrVerrou pVerrou)
{
	DeleteCriticalSection((CRITICAL_SECTION*)pVerrou); // On rend le mutex au systeme
	free(pVerrou);
}

#endif

/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: queue.h
 * Autori: 	Kollar David <xkolla07>
 *			Masarova Maria <xmasar13>
 *			Pristas Jan <xprist06>
 *			Yaremchuk Roman <xyarem00>
 */

 #ifndef QUEUE_H
 #define QUEUE_H

#include "scanner.h"


#define MAX_QUEUE 512



typedef struct Queue {
	int arr[MAX_QUEUE];                        
} tQueue;

bool queueEmpty(tQueue *q);
void queueInit( tQueue *q );
void queueFront ( const tQueue* q, int *symbol );
void queuePop ( tQueue* q );
void queuePush ( tQueue* q, int symbol );
void queueDispose (tQueue *q);
void printQueue (tQueue *q);

#endif

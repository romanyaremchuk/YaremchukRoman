/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: stack.h
 * Autori: 	Kollar David <xkolla07>
 *			Masarova Maria <xmasar13>
 *			Pristas Jan <xprist06>
 *			Yaremchuk Roman <xyarem00>
 */

 #ifndef STACK_H
 #define STACK_H

#include "scanner.h"


#define MAX_STACK 512



typedef struct Stack {
	int arr[MAX_STACK];
	int top;      
	int size;                          
} tStack;


void stackInit ( tStack* s );
int stackEmpty ( const tStack* s );
int stackFull ( const tStack* s );
void stackTop ( const tStack* s, int *symbol );
void stackPop ( tStack* s );
void stackPush ( tStack* s, int symbol );
void stackDispose (tStack *s);
void printStack (tStack *s);

#endif

/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: stack.c
 * Autori: 	Kollar David <xkolla07>
 *			Masarova Maria <xmasar13>
 *			Pristas Jan <xprist06>
 *			Yaremchuk Roman <xyarem00>
 */

#include "stack.h"


int stackSize = MAX_STACK;


void stackInit (tStack* s) {
	s->top = -1;
}

int stackEmpty (const tStack* s) {
	return (s->top == -1 ? 1 : 0);
}

int stackFull (const tStack* s) {
	return ((s->top) == (stackSize - 1));
}

void stackTop (const tStack* s, int *symbol) {
  		*symbol = s->arr[s->top];
}

void stackPop (tStack* s) {
	if ((stackEmpty(s)) != 0) {
  		return;
	} else {
  		s->top = s->top - 1;
  		s->size -= 1;
 	};
}

void stackPush(tStack * s, int symbol) {
  if (stackFull(s) != 0) {
    stackDispose(s);
    exit(EIN);
  } else {
	  s->top = s->top + 1;
	  s->arr[s->top] = symbol;
	  s->size += 1;
  }
}


void stackDispose (tStack *s) {
	free((void*)s);
	s = NULL;
}

void printStack (tStack *s){
	if (!stackEmpty(s)){
		for (int i = 0; i < s->size; i++){
			switch(s->arr[i]){
				case 14:
					printf("$ ");
					break;
				case 16:
					printf("E ");
					break;
				case 13:
					printf("i ");
					break;
				case 0:
					printf("+ ");
					break;
				case 1:
					printf("- ");
					break;
				case 2:
					printf("* ");
					break;
				case 3:
					printf("/ ");
					break;
				case 5:
					printf("< ");
					break;
				case 6:
					printf("> ");
					break;
				case 8:
					printf("<= ");
					break;
				case 9:
					printf(">= ");
					break;
				case 10:
					printf("<> ");
					break;
				case 15:
					printf("< ");
					break;
				case 11:
					printf("( ");
					break;
				case 12:
					printf(") ");
					break;
				case 4:
					printf("\\ ");
					break;
				case 7:
					printf("= ");
					break;
			}
		}
	}
}
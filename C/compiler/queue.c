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

#include "queue.h"

int queueSize = MAX_QUEUE;
int rear = -1;
int front = -1;

bool queueEmpty(tQueue *q){
	if (front == rear){
		return true;
	} else {
		return false;
	}
}

void queueInit( tQueue *q ){
	front = -1;
	rear = -1;
}

void queueFront ( const tQueue* q, int *symbol ){
	*symbol = q->arr[front];
}

void queuePop ( tQueue* q ){
	if (front == -1 || front > rear){
		return;
	} else {
		front = front + 1;
	}
}

void queuePush ( tQueue* q, int symbol ){
	if (rear == queueSize - 1){
		return;
	} else {
		if (front == -1){
			front = 0;
		}

		rear = rear + 1;
		q->arr[rear] = symbol;
	}
}

void queueDispose (tQueue *q){
	free((void*)q);
	q = NULL;
}


void printQueue (tQueue *q){
	int i;
	if (front == -1){
		printf("Queue is empty.\n");
	} else {
		for (i = front; i <= rear; i++){
			switch(q->arr[i]){
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
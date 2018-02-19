/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: str.h
 * Autori: 	Kollar David <xkolla07>
 *			Masarova Maria <xmasar13>
 *			Pristas Jan <xprist06>
 *			Yaremchuk Roman <xyarem00>
 */

 #ifndef STR_H
 #define STR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

 typedef struct
 {
   char* str;
   int length;
   int alloc;
 } string;


int InitString(string *s);
void FreeString(string *s);
void ClearString(string *s);
int AddCharToString(string *s1, char c);
void PrintString(string *s);
char *GetString(string *s);
int GetStringLength(string *s);
int CompareString(string *s1, string *s2);
int CompareStringToChar(string *s1, char* s2);

#endif

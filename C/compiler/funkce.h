/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 27.11.2017
 * Subor: funkce.h
 * Autori: 	Kollar David <xkolla07>
 *			Masarova Maria <xmasar13>
 *			Pristas Jan <xprist06>
 *			Yaremchuk Roman <xyarem00>
 */

 #ifndef FUNKCE_H
 #define FUNKCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>	
#include "str.h"


int length(string s);//velikost
string substr(string s, int i, int n);//poretezec od i
int asc (string s, int i);//hodnota znaku na pozici I
string chr(int i);//jednoprvkovy retezec s hodnotou (acii) I



#endif

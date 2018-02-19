/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: errors.c
 * Autori: 	Kollar David <xkolla07>
 *			Masarova Maria <xmasar13>
 *			Pristas Jan <xprist06>
 *			Yaremchuk Roman <xyarem00>
 */ 
  
#include "parser.h" 

int main(int argc, char** argv)
{   
 	FILE *f = stdin;  
	
	setSourceFile(f);   
	
	Parse();
	fclose(f);

	return EOK;
}
/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 27.11.2017
 * Subor: funkce.c
 * Autori: 	Kollar David <xkolla07>
 *			Masarova Maria <xmasar13>
 *			Pristas Jan <xprist06>
 *			Yaremchuk Roman <xyarem00>
 */

#include "funkce.h"

int length(string s) //vraci delku retezce
{
	return GetStringLength(&s);
}

string substr(string s, int i, int n)
{

	string cutString;//inicializace prazdneho str
	InitString(&cutString);


	int size = length(s); // velikost vstupniho pole


	if ((size == 0) || ((i <= 0) )) 
		return cutString; // jestli je prazdny

	if (n > size - i - 1) // prilis velky kousek. pocitano od 1
	{
		i--; // pocitano od 1
		while (s.str[i] != '\0')
		{
			 AddCharToString(&cutString, s.str[i]);
			 i++;
		}
	return cutString; 
	}


	i--; //pocitano od 1


	while (n != 0) //pokud pole I N jsou normalni
	{
		AddCharToString(&cutString, s.str[i]);
		n--;
	}

	return cutString;		
}



int asc (string s, int i) // vraci hodnotu znaku (ascii) na pozici I
{
	if (length(s) > i)//prilis velke pole
		return -1;

	int result;

	return result = s.str[i];
}



string chr(int i) // nedefinovane chovani ?!
{
	if ((i >= 0) && (i <= 255))
	{
		string charString;
		InitString(&charString);

		char strAdd = i;

		AddCharToString(&charString, strAdd);
		return charString;
	} 
	string charString;
	InitString(&charString);
	return charString;
}

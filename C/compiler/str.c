/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: str.c
 * Autori: 	Kollar David <xkolla07>
 *			Masarova Maria <xmasar13>
 *			Pristas Jan <xprist06>
 *			Yaremchuk Roman <xyarem00>
 */

 #include "str.h"

 int InitString(string *s)
{
   if ((s->str = (char*) malloc(512)) == NULL)
      return 1; // nezdarilo se
   s->str[0] = '\0';
   s->length = 0;
   s->alloc = 10;
   return 0;  // inicializace se zdarila
}

void FreeString(string *s)
{
   free(s->str);
}

void ClearString(string *s)
{
   s->str[0] = '\0';
   s->length = 0;
}

int AddCharToString(string *s1, char c)
{
   if (s1->length + 1 >= s1->alloc)
   {
      if ((s1->str = (char*) realloc(s1->str, s1->length + 10)) == NULL)
         return 1;
      s1->alloc = s1->length + 10;
   }
   s1->str[s1->length] = c;
   s1->length++;
   s1->str[s1->length] = '\0';
   return 0;
}

void PrintString(string *s)
{
   ;
  //printf("%s\n", s->str);
}

char *GetString(string *s)
{
   return s->str;
}

int GetStringLength(string *s)
{
   return s->length;
}

int CompareString(string *s1, string *s2)
{
   return strcmp(s1->str, s2->str);
}

int CompareStringToChar(string *s1, char* s2)
{
   return (strcmp(s1->str, s2) == 0);
}



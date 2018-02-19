/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: errors.h
 * Autori: 	Kollar David <xkolla07>
 *			Masarova Maria <xmasar13>
 *			Pristas Jan <xprist06>
 *			Yaremchuk Roman <xyarem00>
 */

 #ifndef ERRORS_H
 #define ERRORS_H

#include <stdio.h>
#include <stdlib.h>

enum errors {
	EOK = 0, 		// vsetko OK
	ELEX = 1,		// lexikalna chyba
	ESYN = 2,		// syntakticka chyba
	ESEM_DEF = 3,	// semanticka chyba (nedefinvana premenna/funkcia, pokus o redefiniciu)
	ESEM_TKP = 4,	// semanticka chyba (typova kompatibilita, zly pocet parametrov)
	ESEM_OST = 6,	// semanticka chyba (ostatne)
	EIN = 99,		// interna chyba prekladaca
};

void error_print(int err);

#endif

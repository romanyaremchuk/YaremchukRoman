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

#include "errors.h"

void error_print(int err) {
	switch(err){
		case 0:
			fprintf(stderr, "Vsetko OK.\n");
			break;
		case 1:
			fprintf(stderr, "Lexikalna chyba.\n");
			break;
		case 2:
			fprintf(stderr, "Syntakticka chyba.\n");
			break;
		case 3:
			fprintf(stderr, "Semanticka chyba (nedefinovana premenna/funkcia, pokus o redefiniciu).\n");
			break;
		case 4:
			fprintf(stderr, "Semanticka chyba (typova kompatibilita, zly pocet parametrov).\n");
			break;
		case 6:
			fprintf(stderr, "Semanticka chyba (ostatne).\n");
			break;
		case 99:
			fprintf(stderr, "Interna chyba prekladaca.\n");
			break;
	}
}

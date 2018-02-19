/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: parser.h
 * Autori: 	Kollar David <xkolla07>
 *			Masarova Maria <xmasar13>
 *			Pristas Jan <xprist06>
 *			Yaremchuk Roman <xyarem00>
 */

 #ifndef PARSER_H
 #define PARSER_H

#include "precedence.h"

void end_function(int err);
void start();
void func();
void declare(); 
void define();
void func_param();
void func_params();
void func_dim();
void func_body();
void scope();
void scope_body();
void body();
void dim_n();
void printable();
void printable_n();
void expr();
void b_expr();
void return_expr();
void condition();
void func_condition();
void loop();
void eol_check();
void end_scope_check();
int Parse();

#endif

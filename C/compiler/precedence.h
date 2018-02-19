/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: precedence.h
 * Autori: Masarova Maria <xmasar13>
 *      Pristas Jan <xprist06>
 */

#ifndef PROJEKT_PRECEDENCE_H
#define PROJEKT_PRECEDENCE_H

#include "errors.h"
#include "scanner.h"
#include "stack.h"
#include "queue.h"
#include "symtable.h"

enum tab {
    TLESS, // <
    TMORE, // >
    TEQL, // =
    TX // X
};

enum expr {
  EADD, // 0
  ESUB, // 1
  EMUL, // 2
  EDIV, // 3
  EMOD, // 4
  ELESS, // 5
  EGRT, // 6
  EEQL, // 7
  EEQLL, // 8
  EEQLG, // 9
  EDIF, // 10
  ELB, // 11
  ERB, // 12
  EI, // 13
  EDOLLAR, // 14
  ESHIFT, //15
  E //16
};

int precedence(int b_flag, SToken token, char * function_name, tTable * glob_table, union v_f data, int variable_type, char * variable_name);
void reduce ();
void shift ();

#endif //PROJEKT_PRECEDENCE_H

/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: parser.h
 * Autori: 	Masarova Maria <xmasar13>
 *			Pristas Jan <xprist06>
 */

 #ifndef SYMTABLE_H
 #define SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define MAX_SIZE 101

union v_f {
	struct variable *var;
	struct function *func;
};

typedef enum tInfo {
	VAR,
	FUNC,
} tInfo;

typedef enum tType {
	t_INT,
	t_DOUBLE,
	t_STRING,
} tType;

typedef struct tItem {
	char * key;
	union v_f data;
	enum tInfo info;
	struct tItem* next;
} tItem;

typedef struct tTable {
	int size;
	struct tItem *ptr[];
} tTable;

typedef struct variable {
	char * var_name;
	enum tType type;
	union {
		int int_val;
		double double_val;
		char * string_val;
	} val;
	bool defined;
} tVar;

typedef struct tParams {
	enum tType type;
	char * param_name;
} tParams;

typedef struct function {
	char * func_name;
	enum tType return_type;
	bool declared;
	bool defined;
	struct tParams **params;
	int count_params;
	struct tTable *local_table;
} tFunc;

unsigned int hashCode(tTable *table, char * key);

tTable * tableInit(int size);

tItem * tableSearch(tTable * table, char * key);

void tableInsert(tTable * table, char * key, union v_f data, enum tInfo info);

void tableEdit(tTable * table, char * key, union v_f data);

void tableDelete(tTable * table, char * key);

void tableClearAll(tTable * table);

void tablePrint(tTable * table);

#endif

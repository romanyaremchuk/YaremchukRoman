/*
 * Projekt na IFJ a IAL
 * Varianta projektu: II
 * Datum: 22.9.2017
 * Subor: scanner.h
 * Autori: 	Kollar David <xkolla07>
 *			Masarova Maria <xmasar13>
 *			Pristas Jan <xprist06>
 *			Yaremchuk Roman <xyarem00>
 */

 #ifndef SCANNER_H
 #define SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "errors.h"
#include "str.h"



typedef enum {
	state_start,
	state_string, //!"
	state_greater, // >
	state_less, // <
	state_id,
	state_int,
    state_double,
    state_double1,
    state_exponent,
	state_exponent2,
	state_exponent3,
    state_escape,
    state_error,
    state_end,
    state_array,
    state_comment,
    state_comment1,
    state_comment_line
} States;

typedef enum Token_type {
    token_eol, //0
    token_slash, //1
    token_assignment, //2
    token_leftBracket, //3
    token_rightBracket, //4
    token_less, //5
    token_lessOrEq, //6
    token_greater, //7
    token_greaOrEq, //8
    token_notEqual, //9
    token_colom, //10
    token_semicolom, //11
    token_error, //12
    token_true, //13
    token_static, //14
    token_shared, //15
    token_or, //16
    token_not, //17
    token_next, //18
    token_for, //19
    token_false, //20
    token_exit, //21
    token_else_if, //22
    token_continue, //23
    token_boolean, //24
    token_and, //25
    token_substr, //26
    token_return, //27
    token_loop, //28
    token_lenght, //29
    token_chr, //30
    token_scope, //31
    token_end_scope, //32
    token_declare, //33
    token_define, //34
    token_function, //35
    token_end_function, //36
    token_dim, //37
    token_id, //38
    token_as, //39
    token_asc, //40
    token_print, //41
    token_input, //42
    token_do, //43
    token_while, //44
    token_if, //45
    token_then, //46
    token_else, //47
    token_end_if, //48
    token_integer, //49
    token_double, //50
    token_string, //51
    token_asgnm, //52
    token_val, //53
    token_plus, //54
    token_minus, //55
    token_mul, //56
    token_div, //57
    token_mod, //58
    token_eof, //59
} Token_type;

typedef struct Token_value {
    int number;
    char *array;
    double number_double;
} Token_value;

typedef struct token {
    Token_value value;
    int var_type;
    Token_type type;
    char * name;
} SToken;


typedef enum State {
    ERROR,
    TRUE,
    STATIC,
    SHARED,
    OR,
    NOT,
    NEXT,
    FOR,
    FALSE,
    EXIT,
    ELSE_IF,
    CONTINUE,
    BOOLEAN,
    AND,
    SUBSTR,
    RETURN,
    LOOP,
    LENGHT,
    CHR,
    SCOPE,
    END_SCOPE,
    DECLARE,//
    DEFINE,
    FUNCTION,//
    END_FUNCTION,
    DIM,//
    ID,
    AS,//
    ASC,
    PRINT,//
    INPUT,//
    DO,//
    WHILE,//
    IF,//
    THEN,//
    ELSE,//
    END_IF,
    INTEGER,//
    DOUBLE,//
    STRING,//
    ASGNM,
    VAL,//--
    PLUS,//
    MINUS,//
    MUL,//
    DIV,//
    MOD,///!
    LESS,//
    L_EQL,//
    GREATER,//
    G_EQL,//
    EQUAL,//
    NOT_EQL,//
    LEFT_BRACKET,//
    RIGHT_BRACKET,//
    COLOM, ///-----------------------------------------------^
    SEMICOLOM,//
    COMMENT,
    BLCK_COMNT,
    EOL
} State;



void setSourceFile(FILE *f);
SToken GetToken();
//int nextToken(string* text);

#endif


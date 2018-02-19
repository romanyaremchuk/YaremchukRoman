/*
*************************************|
* mylib.c                            |
* Solution IOS-DU2 : Child Care      |
* Author: Yaremchuk Roman (xyarem00) |
* Date: 28.04.2017                   |
*************************************|
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include "mylib.h"

/* Parse input options */
int get_opts(int argc, char const *argv[], unsigned opts[]) {
    /* Help message */
    char const *help = "Usage: $ ./proj2 A C AGT CGT AWT CWT\n"
    "    A   number of adults; A > 0.\n"
    "    C   number of children; C > 0.\n"
    "    AGT max time(ms) of generating the next adult; AGT >= 0 && AGT < 5001.\n"
    "    CGT max time(ms) of generating the next child; AGT >= 0 && AGT < 5001.\n"
    "    AWT max activity time(ms) for adult.\n"
    "    CWT max activity time(ms) for child.\n"
    "    All parameters are integers.\n";
    /* Error flag (0 = no error) */
    int err = 0;
    if (argc != 7)
        err = 1;
    else {
        if (!str_to_ul(argv[1], &opts[A]) || opts[A] == 0)
            err = 1;
        if (!str_to_ul(argv[2], &opts[C]) || opts[C] == 0)
            err = 1;
        if (!str_to_ul(argv[3], &opts[AGT]) || opts[AGT] > 5000)
            err = 1;
        if (!str_to_ul(argv[4], &opts[CGT]) || opts[CGT] > 5000)
            err = 1;
        if (!str_to_ul(argv[5], &opts[CGT]) || opts[CGT] > 5000)
            err = 1;
        if (!str_to_ul(argv[6], &opts[CGT]) || opts[CGT] > 5000)
            err = 1;
    }
    if (err) {
        fprintf(stderr, "%s", help);
        return 0;
    }
    return 1;
}

/* Modified strtoul */
int str_to_ul(char const *str, unsigned *num) {
    char *garbage;
    *num = strtoul(str, &garbage, 10);
    if ((*str == '-') || (strcmp(garbage, "") != 0) || (errno == ERANGE)) {
        return 0;
    }
    return 1;
}

/* Vytvari nahodne cislo */
unsigned get_rand_num(unsigned from) {
    if (from != 0) {
        return (unsigned)(rand()%(from + 1));
    }
    return 0;
}

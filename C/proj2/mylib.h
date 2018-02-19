/*
*************************************|
* mylib.h                            |
* Solution IOS-DU2 : Child Care      |
* Author: Yaremchuk Roman (xyarem00) |
* Date: 28.04.2017                   |
*************************************|
*/
#ifndef MYLIB_H
#define MYLIB_H

/* Constants for indexing the options array */
#define A 0   /* index for adults */
#define C 1   /* index for children */
#define AGT 2 /* index for adults generator */
#define CGT 3 /* index for children generator */
#define AWT 4 /* index for adults time activity */
#define CWT 5 /* index for children time activity */

#define GET_MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

/* Parse input options */
int get_opts(int argc, char const *argv[], unsigned opts[]);
/* Modified strtoul */
int str_to_ul(char const *str, unsigned *num);
/* Get random number */
unsigned get_rand_num(unsigned from);

#endif /* MYLIB_H */

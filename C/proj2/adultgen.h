/*
*************************************|
* adultgen.h                         |
* Solution IOS-DU2 : Child Care      |
* Author: Yaremchuk Roman (xyarem00) |
* Date: 28.04.2017                   |
*************************************|
*/
#ifndef ADULTGEN_H
#define ADULTGEN_H

#include <sys/types.h>

/* Create process 'adult' */
int generate_adult(pid_t *a_pid, int id, unsigned opts[]);
/* Start adults generator */
int start_adult_gen(pid_t *a_gen_pid, unsigned opts[]);

#endif /* ADULTGEN_H */

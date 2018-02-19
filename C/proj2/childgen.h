/*
*************************************|
* childgen.h                         |
* Solution IOS-DU2 : Child Care      |
* Author: Yaremchuk Roman (xyarem00) |
* Date: 28.04.2017                   |
*************************************|
*/
#ifndef CHILDGEN_H
#define CHILDGEN_H

#include <sys/types.h>

/* Create process 'adult' */
int generate_child(pid_t *c_pid, int id, unsigned opts[]);
/* Start adults generator */
int start_child_gen(pid_t *c_gen_pid, unsigned opts[]);

#endif /* CHILDGEN_H */

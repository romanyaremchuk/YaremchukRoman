/*
*************************************|
* proj2.c                            |
* Solution IOS-DU2 : Child Care      |
* Author: Yaremchuk Roman (xyarem00) |
* Date: 28.04.2017                   |
*************************************|
*/
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "mylib.h"
#include "shmsem.h"
#include "adultgen.h"
#include "childgen.h"

int main(int argc, char const* argv[]) {
    /* Application options */
    unsigned *opts = (unsigned *)malloc(sizeof(unsigned)*6);
    /* If there is at least 1 worng argument, help is printed to stderr */
    if (!get_opts(argc, argv, opts)) {
        free(opts);
        return 1;
    }
    /* Create log file "./proj2.out" */
    if (!create_log_file()) {
        free(opts);
        return 2;
    }
    /* Create semaphores and shared memory */
    if (!create_shmsem()) {
        free(opts);
        abortion();
        return 2;
    }
    pid_t a_gen_pid;
    /* Start adults generator */
    if (!start_adult_gen(&a_gen_pid, opts)) {
        free(opts);
        abortion();
        return 2;
    }
    pid_t c_gen_pid;
    /* Start children generator */
    if (!start_child_gen(&c_gen_pid, opts)) {
        free(opts);
        abortion();
        return 2;
    }
    /* Wait for all generators */
    waitpid(a_gen_pid, NULL, 0);
    waitpid(c_gen_pid, NULL, 0);
    /* Free memory */
    free(opts);
    remove_shmsem();
    if (!close_log_file())
        return 2;
    return 0;
}

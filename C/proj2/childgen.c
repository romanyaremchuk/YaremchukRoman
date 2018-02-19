/*
*************************************|
* childgen.c                         |
* Solution IOS-DU2 : Child Care      |
* Author: Yaremchuk Roman (xyarem00) |
* Date: 28.04.2017                   |
*************************************|
*/
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shmsem.h"
#include "mylib.h"
#include "childgen.h"

/* Create process 'adult' */
int generate_child(pid_t *c_pid, int id, unsigned opts[]) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        add_to_log("C", id, "started\n");

        sem_wait(g_sem_mx);
        if (shm_data->children < 3*shm_data->adults
            || shm_data->gone_adults == opts[A])
        {
            add_to_log("C", id, "enter\n");
            ++shm_data->children;
            sem_post(g_sem_mx);
        }
        else {
            add_to_log("C", id, "waiting : %d : %d\n", shm_data->adults,
                shm_data->children);
            ++shm_data->waiting;
            sem_post(g_sem_mx);
            sem_wait(g_sem_cq);
            add_to_log("C", id, "enter\n");
        }

        usleep(1000*get_rand_num(opts[CWT]));
        add_to_log("C", id, "trying to leave\n");

        sem_wait(g_sem_mx);
        add_to_log("C", id, "leave\n");
        ++shm_data->gone_children;
        --shm_data->children;
        if (shm_data->leaving && shm_data->children <= 3*(shm_data->adults-1)) {
            --shm_data->leaving;
            --shm_data->adults;
            sem_post(g_sem_aq);
        }
        sem_post(g_sem_mx);

        sem_wait(g_sem_smx);
        if (shm_data->gone_children == opts[C]) {
            if (shm_data->gone_adults == opts[A]) {
                for (unsigned i = 0; i < opts[A] + opts[C]; i++) {
                    sem_post(g_sem_wa);
                }
            }
            else {
                sem_wait(g_sem_awa);
                for (unsigned i = 0; i < opts[A] + opts[C]; i++) {
                    sem_post(g_sem_wa);
                }
            }
        }
        sem_post(g_sem_smx);

        sem_wait(g_sem_wa);
        add_to_log("C", id, "finished\n");
        exit(0);
    }
    if (pid < 0) {
        perror("Cannot create child.\n");
        return 0;
    }
    if (pid > 0) {
        *c_pid = pid;
    }
    return 1;
}

/* Start adults generator */
int start_child_gen(pid_t *c_gen_pid, unsigned opts[]) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        pid_t c_pid;
        for (unsigned id = 1; id <= opts[C]; id++) {
            usleep(1000*get_rand_num(opts[CGT]));
            if (!generate_child(&c_pid, id, opts)) {
                exit(2);
            }
        }
        for (unsigned i = 0; i < opts[C]; i++) {
            waitpid(c_pid, NULL, 0);
        }
        exit(0);
    }
    if (pid < 0) {
        perror("Cannot create children generator.\n");
        return 0;
    }
    if (pid > 0) {
        *c_gen_pid = pid;
    }
    return 1;
}

/*
*************************************|
* adultgen.c                         |
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
#include "adultgen.h"

/* Create process 'adult' */
int generate_adult(pid_t *a_pid, int id, unsigned opts[]) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        add_to_log("A", id, "started\n");

        sem_wait(g_sem_mx);
        add_to_log("A", id, "enter\n");
        ++shm_data->adults;
        if (shm_data->waiting) {
            int n = GET_MIN(3, shm_data->waiting);
            for (int i = n; i > 0; i--) {
                sem_post(g_sem_cq);
            }
            shm_data->waiting -= n;
            shm_data->children += n;
        }
        sem_post(g_sem_mx);

        usleep(1000*get_rand_num(opts[AWT]));
        add_to_log("A", id, "trying to leave\n");
        
        sem_wait(g_sem_mx);
        if (shm_data->children <= 3*(shm_data->adults-1)) {
            add_to_log("A", id, "leave\n");
            --shm_data->adults;
            ++shm_data->gone_adults;
            if (shm_data->gone_adults == opts[A]) {
                sem_post(g_sem_cq);
                sem_post(g_sem_awa);
            }
            sem_post(g_sem_mx);
        }
        else {
            add_to_log("A", id, "waiting : %d : %d\n",
                shm_data->adults, shm_data->children);
            ++shm_data->leaving;
            sem_post(g_sem_mx);
            sem_wait(g_sem_aq);
            add_to_log("A", id, "leave\n");
            ++shm_data->gone_adults;
            if (shm_data->gone_adults == opts[A]) {
                sem_post(g_sem_cq);
                sem_post(g_sem_awa);
            }
        }

        sem_wait(g_sem_wa);
        add_to_log("A", id, "finished\n");
        exit(0);
    }
    if (pid < 0) {
        perror("Cannot create adult.\n");
        return 0;
    }
    if (pid > 0) {
        *a_pid = pid;
    }
    return 1;
}

/* Start adults generator */
int start_adult_gen(pid_t *a_gen_pid, unsigned opts[]) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        pid_t a_pid;
        for (unsigned id = 1; id <= opts[A]; id++) {
            usleep(1000*get_rand_num(opts[AGT]));
            if (!generate_adult(&a_pid, id, opts)) {
                exit(2);
            }
        }
        for (unsigned i = 0; i < opts[A]; i++) {
            waitpid(a_pid, NULL, 0);
        }
        exit(0);
    }
    if (pid < 0) {
        perror("Cannot create adults generator.\n");
        return 0;
    }
    if (pid > 0) {
        *a_gen_pid = pid;
    }
    return 1;
}

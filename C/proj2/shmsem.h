/*
*************************************|
* shmsem.h                           |
* Solution IOS-DU2 : Child Care      |
* Author: Yaremchuk Roman (xyarem00) |
* Date: 28.04.2017                   |
*************************************|
*/
#ifndef SHMSEM_H
#define SHMSEM_H

#include <stdio.h>
#include <semaphore.h>

#define G_SEM_MUTEX "/mutex_sem_ry"
#define G_SEM_SMUTEX "/smutex_sem_ry"
#define G_SEM_ADULT_QUEUE "/adult_queue_sem_ry"
#define G_SEM_CHILD_QUEUE "/child_queue_sem_ry"
#define G_SEM_LOG "/log_sem_ry"
#define G_SEM_WAIT_ALL "/wait_all_sem_ry"
#define G_SEM_WAIT_ALL_ADULTS "/wait_adults_sem_ry"

#define G_SHM "/child_care_shm_ry"

#define G_LOG_FILE "proj2.out"

/* Semaphores */
sem_t* g_sem_mx;  /* mutex */
sem_t* g_sem_smx; /* second mutex */
sem_t* g_sem_cq;  /* child queue */
sem_t* g_sem_aq;  /* adult queue */
sem_t* g_sem_log; /* log file */
sem_t* g_sem_wa;  /* wait all */
sem_t* g_sem_awa; /* wait all adulst */

/* Shared memory */
typedef struct shm {
    unsigned action;         /* action counter */
    unsigned adults;         /* present adults */
    unsigned children;       /* present children */
    unsigned leaving;        /* leaving people */
    unsigned waiting;        /* waiting people */
    unsigned gone_adults;    /* gone adults */
    unsigned gone_children;  /* gone children */
} shm_t;

shm_t* shm_data;
/* Create log file */
int create_log_file();
/* Close log file */
int close_log_file();
/* Create shared memory and semaphores */
int create_shmsem();
/* Remove shared memory and semaphores */
void remove_shmsem();
/* Abort operations */
void abortion();
/* Print to log file */
void add_to_log(char const* process, int id, char const* message, ...);

#endif /* SHMSEM_H */

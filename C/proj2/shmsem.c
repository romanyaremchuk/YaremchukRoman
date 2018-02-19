/*
*************************************|
* shmsem.c                           |
* Solution IOS-DU2 : Child Care      |
* Author: Yaremchuk Roman (xyarem00) |
* Date: 28.04.2017                   |
*************************************|
*/
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include "shmsem.h"

static FILE *g_log_file;

/* Create shared memory and semaphores */
int create_shmsem() {
    /* Shared memory */
    int g_shm_fd;
    if ((g_shm_fd = shm_open(G_SHM, O_CREAT | O_RDWR, 0666)) < 0 ) {
        perror("The shm_open() function error");
        return 0;
    }
    if (ftruncate(g_shm_fd, sizeof(shm_t)) != 0 ) {
        perror("The ftruncate() function error");
        return 0;
    }
    if ((shm_data = (shm_t*)mmap(NULL, sizeof(shm_t),
        PROT_WRITE | PROT_READ, MAP_SHARED, g_shm_fd, 0)) == MAP_FAILED )
    {
        perror("The mmap() function error");
        return 0;
    }
    close(g_shm_fd);
    /* Semaphores */
    if ((g_sem_mx = sem_open(G_SEM_MUTEX,
            O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED
    ) {
        perror("The sem_open() function error");
        return 0;
    }
    if ((g_sem_smx = sem_open(G_SEM_SMUTEX,
            O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED
    ) {
        perror("The sem_open() function error");
        return 0;
    }
    if ((g_sem_cq = sem_open(G_SEM_CHILD_QUEUE,
            O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED
    ) {
        perror("The sem_open() function error");
        return 0;
    }
    if ((g_sem_aq = sem_open(G_SEM_ADULT_QUEUE,
            O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED
    ) {
        perror("The sem_open() function error");
        return 0;
    }
    if ((g_sem_log = sem_open(G_SEM_LOG,
            O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED
    ) {
        perror("The sem_open() function error");
        return 0;
    }
    if ((g_sem_wa = sem_open(G_SEM_WAIT_ALL,
            O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED
    ) {
        perror("The sem_open() function error");
        return 0;
    }
    if ((g_sem_awa = sem_open(G_SEM_WAIT_ALL_ADULTS,
            O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED
    ) {
        perror("The sem_open() function error");
        return 0;
    }
    return 1;
}

/* Create log file */
int create_log_file() {
    g_log_file = fopen(G_LOG_FILE, "w");
    if (g_log_file == NULL) {
        fprintf(stderr, "Cannot open/create file %s.\n", G_LOG_FILE);
        return 0;
    }
    return 1;
}

/* Close log file */
int close_log_file() {
    if (fclose(g_log_file) != 0) {
        fprintf(stderr, "Cannot close file %s.\n", G_LOG_FILE);
        return 0;
    }
    return 1;
}

/* Print to log file */
void add_to_log(char const* process, int id, char const* message, ...) {
    sem_wait(g_sem_log);
    va_list additional;
    va_start(additional, message);
    fprintf(g_log_file, "%u\t: %s %d\t: ", ++(shm_data->action), process, id);
    vfprintf(g_log_file, message, additional);
    fflush(g_log_file);
    va_end(additional);
    sem_post(g_sem_log);
}

/* Abort operations */
void abortion() {
    close_log_file();
    remove_shmsem();
    fprintf(stderr, "Something went wrong, all allocations are cleared.\n");
}

/* Remove shared memory and semaphores */
void remove_shmsem() {
    /* Shared memory */
    munmap(shm_data, sizeof(shm_t));
    shm_unlink(G_SHM);
    /* Semaphores */
    sem_close(g_sem_mx);
    sem_close(g_sem_smx);
    sem_close(g_sem_aq);
    sem_close(g_sem_cq);
    sem_close(g_sem_log);
    sem_close(g_sem_wa);
    sem_close(g_sem_awa);

    sem_unlink(G_SEM_MUTEX);
    sem_unlink(G_SEM_SMUTEX);
    sem_unlink(G_SEM_ADULT_QUEUE);
    sem_unlink(G_SEM_CHILD_QUEUE);
    sem_unlink(G_SEM_LOG);
    sem_unlink(G_SEM_WAIT_ALL);
    sem_unlink(G_SEM_WAIT_ALL_ADULTS);
}

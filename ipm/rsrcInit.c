#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_MODE        600
#define SHM_MAT_FLAG      0

int main(void)
{
    key_t key = ftok("./rsrc", 1);
printf("%d\n", key);
    if (key == -1)
    {
        fprintf(stderr, "ftok Failed...\n");    exit(-1);
    }

    int shmidMutexRsrc;
    if ((shmidMutexRsrc = shmget(key, sizeof(pthread_mutex_t), SHM_MODE)) == -1)
    {
        fprintf(stderr, "shmget Failed\n"); exit(-2);
    }
printf("shmget = %p\n", shmidMutexRsrc);

    pthread_mutex_t * pMutexAccess;
    if ((pMutexAccess = shmat(shmidMutexRsrc, NULL, SHM_MAT_FLAG)) == (void *) -1)
    {
        fprintf(stderr, "shmat Failed\n");  exit(-3);
    }

    int ret = 0;
    pthread_mutexattr_t mutex_attr;
    ret |= pthread_mutexattr_init(&mutex_attr);
    ret |= pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    ret |= pthread_mutex_init(pMutexAccess, &mutex_attr);
    ret |= pthread_mutexattr_destroy(&mutex_attr);
    if (ret != 0)
    {
        fprintf(stderr, "\n");  exit(-4);
    }

    printf("Resource Init succeed...\n");

    exit(0);
}

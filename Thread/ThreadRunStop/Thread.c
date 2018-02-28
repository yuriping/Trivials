#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "main.h"

static void * Thread0(void * prm)
{
    static unsigned long count = 0;
    while (thr0.run)
    {
	count ++;
        printf("thread 0 count = %ld\n", count);
        sleep(1);
    }

    pthread_exit((void *)0);
}

static void * Thread1(void * prm)
{
    static unsigned long count = 0;
    while (1)
    {
	count ++;
        printf("thread 11111 count = %ld\n", count);
        sleep(1);
    }

    pthread_exit((void *)1);
}

void ThreadStart()
{
    thr0.run = 1;
    pthread_create(&(thr0.tid), NULL, Thread0, NULL);

    pthread_t thr1;
    pthread_create(&thr1, NULL, Thread1, NULL);
}

void ThreadStop()
{
    void * retval;

    thr0.run = 0;
    pthread_join(thr0.tid, &retval);
    printf("thread ret val = %d\n", (unsigned int)retval);
}



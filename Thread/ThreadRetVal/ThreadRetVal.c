#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct BODY {
    unsigned char ch;
    unsigned int  i;
} BODY, * P_BODY;

/*
void * thr(void * arg)
{
    static unsigned long count = 0;

    while (count < 5)
    {
        count ++;
        printf("thread 1 count = %ld\n", count);
        sleep(1);
    }

    P_BODY pbody = (P_BODY) malloc(sizeof(BODY));
    pbody->ch = 'y';
    pbody->i  = count;

    pthread_exit((void *) pbody);
}//*/

void * thr(void * arg)
{
    static unsigned long count = 0;

    while (count < 5)
    {
        count ++;
        printf("thread 1 count = %ld\n", count);
        sleep(1);
    }

    P_BODY pbody = (P_BODY) malloc(sizeof(BODY));
    pbody->ch = 'y';
    pbody->i  = count;

    pthread_exit((void *)-4);
}

int main(void)
{
    pthread_t tid;
    void * ret;
    pthread_create(&tid, NULL, thr, NULL);

    pthread_join(tid, &ret);
    P_BODY pbody = (P_BODY)ret;
    //printf("thread exit code : ch = %c, i = %d\n", pbody->ch, pbody->i);
    printf("thread exit code : %d\n", (int)ret);

    return 0;
}


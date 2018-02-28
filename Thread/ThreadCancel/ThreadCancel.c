#include <stdio.h>
#include <pthread.h>

//test thread 1 cancel thread 2, 2 manners:1-using cancel, 2-using a volatile flag, prefer to the latter...

pthread_t tid1, tid2;
void * ret;
volatile unsigned int thr2_run;

void * thr_fxn1(void * arg)
{
    static unsigned long count = 0;
    while (count < 10)
    {
	count ++;
	if (5 == count)
            //thr2_run = 0;		//manner 2
            pthread_cancel(tid2);	//manner 1
        printf("thread 1 count = %ld\n", count);
	sleep(1);
    }

    pthread_exit((void *)1);
}

void * thr_fxn2(void * arg)
{
    static unsigned long count = 0;
    //while (thr2_run)	//manner2
    while (count < 15)	//manner1
    {
	count ++;
        printf("thread 2 count = %ld\n", count);
        sleep(1);
    }

    return (void *) 2;
}

int main(void)
{
    thr2_run = 1;

    pthread_create(&tid1, NULL, thr_fxn1, NULL);
    pthread_create(&tid2, NULL, thr_fxn2, NULL);
/*
    pthread_join(tid1, &ret);
    printf("thread 1 exit code : %d\n", (int)ret);	//*/
    pthread_join(tid2, &ret);
    printf("thread 2 exit code : %d\n", (int)ret);

    pthread_join(tid1, &ret);
    printf("thread 1 exit code : %d\n", (int)ret);

    return 0;
}


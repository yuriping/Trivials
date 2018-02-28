#include <stdio.h>
#include <pthread.h>

void * thread0(void * prm)
{
    unsigned int count = 0;
    while(1)
    {
	count ++;
	printf("thread 0 , count = %d\n", count);
        //sleep(1);
    }

    pthread_exit((void *)0);
}

void * thread1(void * prm)
{
    unsigned int count = 0;
    while(1)
    {
	count ++;
	printf("thread 1 , count = %d\n", count);
        //sleep(1);
    }

    pthread_exit((void *)1);
}

int main(void)
{
    pthread_t tid0, tid1;
    void * retval;

    pthread_create(&tid0, NULL, thread0, NULL);
    pthread_create(&tid1, NULL, thread1, NULL);

    pthread_join(tid0, &retval);
    printf("retval = %d\n", (int)retval);

    pthread_join(tid1, &retval);
    printf("retval = %d\n", (int)retval);

    return 0;
}



#include <stdio.h>
#include <pthread.h>

void * thread0(void * prm)
{
    unsigned int count = 0;
    while(count < 5)
    {
	count ++;
	printf("thread 0 , count = %d\n", count);
        sleep(1);
    }

    pthread_exit((void *)2);
}

int main(void)
{
    pthread_t tid;
    void * retval;

    pthread_create(&tid, NULL, thread0, NULL);

    sleep(10);

    pthread_join(tid, &retval);
    printf("retval = %d\n", (int)retval);

    return 0;
}



/*
 * 测试线程返回前取得自己的Id
 */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


pthread_t tidself = 0;

void * thr(void * prm)
{
    int i = 0;
    for (i = 0; i < 5; i ++)
    {
        printf("i = %d\n", i);

        sleep(1);
    }

    tidself = pthread_self();

    pthread_exit((void *)0);
}

int main(void)
{
    printf("tid (self) : %lu\n", tidself);

    pthread_t tid;
    pthread_create(&tid, NULL, thr, NULL);
    printf("tid (create ret) : %lu\n", tid);

    void * ret;
    pthread_join(tid, &ret);
    printf("tid (self) : %lu\n", tidself);

    for (;;)
        sleep(60);

    exit(0);
}

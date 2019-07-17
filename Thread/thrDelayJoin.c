/*
 * 验证：当线程先 pthread_exit 退出后，pthread_join 并不立即运行，是否能收集到线程的退出信息，并收尸？能……
 */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


pthread_t tid0;


void * thr0(void * prm)
{
    int cnt = 0;

    for (cnt = 0; cnt < 5; cnt ++)
    {
        printf("<thr0> cnt = %d\n", cnt);

        sleep(1);
    }

    int * pret = (int *)malloc(sizeof(int));
    *pret = 5;

    pthread_exit((void *)pret);
}

int main(void)
{
    pthread_create(&tid0, NULL, thr0, NULL);

    sleep(10);

    void * ret;
    pthread_join(tid0, &ret);
    printf("ret = %d\n", *((int *)ret));
    free(ret);

    for (;;)
        sleep(1);

    exit(0);
}

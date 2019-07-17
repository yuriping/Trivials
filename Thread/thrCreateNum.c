/*
 * 目标：测试线程数到了最大值后是否就不能创建新的线程了，如果每个线程都join，是否就可能不断创建
 * 结果：
 *   0. 达到最大数量后，如果继续 pthread_create，则出错，返回值 11，表示资源暂时不够；
 *   1. 如果让之前创建的线程在执行完后立刻退出，但不join，数量比每个线程都不退出时多一些，但仍然到了一定数量后就不能再分配线程了；
 *   2. 如果让每个线程执行完退出，且join，则可以连续工作，这是最正常的方式，要采用！
 *
 *                               Ubuntu        tina
 * 最大进程数（函数）：                7770        6007
 * 最大进程数（proc）：             131072        4096
 * 最大进程数（ulimit）：             7770        6007
 * 最大线程数（proc）：              15540       12014
 * 最大线程数（ulimit）：             7770        6007
 * 最大线程数(实测，不exit):         12059        3780
 * 最大线程数(实测，exit不join):     32751       32760
 * 最大线程数(实测，exit,join):      连续工作     连续工作
 */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


pthread_mutex_t lock;
int cnt = 0;


void * thr(void * prm)
{
    pthread_mutex_lock(&lock);
    cnt ++;
    printf("%d\n", cnt);
    pthread_mutex_unlock(&lock);

//    for (;;)
  //      sleep(60);

    pthread_exit((void *)0);
}

int main(void)
{
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutex_init(&lock, &mutex_attr);

    cnt = 0;

    pthread_t tid;
    for (;;)
    {
        int ret = pthread_create(&tid, NULL, thr, NULL);
        void * retprm;
        pthread_join(tid, &retprm);
//        printf("ret = %d ---- \n", ret);
    }

    for (;;)
        sleep(60);

    exit(0);
}

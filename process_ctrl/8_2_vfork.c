/*
 * vfork 演示：
 *   0. 子进程在父进程的空间内运行，不复制父进程的空间；
 *   1. 子进程先执行，执行完后，父进程才运行；
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int glob = 6;


int main()
{
    int var = 88;
    pid_t pid;

    printf("before fork\n");

    /* 出错 */
    if ((pid = vfork()) < 0)
    {
        printf("vfork error...\n");      exit(1);
    }
    /* 子进程 */
    else if (pid == 0)
    {
        glob ++;
        var ++;
//        _exit(0);     // TODO : 先不讨论 IO 冲洗，下一章节再说，没有这个，父子进程都输出，变量值相同
    }
    // 不用父进程等待，让子进程先执行，子进程就会先执行，执行完，父进程才执行

    /* 如果父子进程都打印，则变量值是一样的，是被子进程改变后的值； */
    printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);

    exit(0);
}

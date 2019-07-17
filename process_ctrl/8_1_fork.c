/*
 * fork 演示：搞清楚
 *   0. 父子进程中返回值不同；
 *   1. 内存复制，子进程修改变量不会影响到父进程；
 *   2. printf的次数会随输出到标准输出和文件有所不同；
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int glob = 6;
char buf[] = "a write to stdout\n";


int main()
{
    int var = 88;
    pid_t pid;

    /* write 无缓冲，所有无论输出到标准输出还是文件，仅一次 */
    write(STDOUT_FILENO, buf, sizeof(buf) - 1);

    /*
     *   标准 IO 连接终端设备（标准输出）行缓冲，文件全缓冲；
     *   定向到标准输出时，换行符会冲洗缓冲区，打印一次；
     *   定向到文件时，父进程打印一次，但在缓冲区中没有实际输出，
     * fork 出子进程时，将缓冲区复制给子进程，两个进程 exit 时，会分别冲洗缓冲区，执行实际输出，所以打印了 2 次；
     */
    printf("before fork\n");

    /* 出错 */
    if ((pid = fork()) < 0)
    {
        printf("fork error...\n");      exit(1);
    }
    /* 子进程 */
    else if (pid == 0)
    {
        glob ++;
        var ++;
    }
    /* 父进程 */
    else // (pid > 0)
    {
        sleep(2);   // 等子进程先执行
    }

    /* 父子进程分别输出，全局变量、局部变量，子进程的修改不会影响到父进程中的值，因为子进程是父进程的复本，不相关； */
    printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);

    exit(0);
}

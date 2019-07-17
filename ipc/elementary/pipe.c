/*
 * 最简单的管道用法示例，父进程向子进程发送信息
 *   0. pipe 是半双工的；
 *   1. pipe 返回一个描述符数组，下标 0 读，1 写；
 *   2. pipe 成功返回 0，失败返回 -1；
 *   3. 管道描述符可被子进程继承；
 *   4. 父子进程应关闭自己不用的一个描述符，专注于读或写；
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define N       (64)    // 可从管道读的最多字节数，即缓存大小


int main()
{
    int ret;        // pipe 函数返回值
    int fp[2];      // 管道的两个文件描述符
    int n;          // 从管道读的字节数
    char buf[N];    // 从管道读的缓存

    /*
     * 创建管道
     */
    if ((ret = pipe(fp)) != 0) // 成功 0，失败 -1
    {
        printf("pipe error...\n");  exit(1);
    }
    // 管道的读写描述符，可被子进程继承，0 读 1 写
    printf("(main) [0] = %d, [1] = %d\n", fp[0], fp[1]);

    /*
     * 创建子进程
     */
    pid_t pid;
    if ((pid = fork()) < 0)     // 子进程创建失败
    {
        printf("fork error...\n");  exit(2);
    }
    else if (pid == 0)          //child
    {
        printf("(Child) [0] = %d, [1] = %d\n", fp[0], fp[1]);     // 子进程继承父进程的文件描述符

        /* 接收父进程数据，关闭写描述符 */
        close(fp[1]);
        /* 从父进程接收数据 */
        n = read(fp[0], buf, N);   // 得到实际读到的字节数
        buf[n] = 0; // 收到的数据加结束符
        // 打印父进程发来的数据
        printf("[child] %s", buf);
    }
    else            // pid > 0  // parent
    {
        printf("(Parent) [0] %d, [1] = %d\n", fp[0], fp[1]);

        /* 向子进程发数据，关闭读描述符 */
        close(fp[0]);
        /* 向子进程传递数据 */
        write(fp[1], "Hello world\n", sizeof("Hello world\n"));
    }

    exit(0);
}

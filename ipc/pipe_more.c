/*
 * 演示功能：
 * 使用匿名管道实现向既有分页程序 more 传送文件
 *
 * 工作原理描述：
 * 父进程向管道写端写入文件内容，子进程启动一个现有的 more 分页程序，more 分页程序从自己的标准输入文件得到数据去显示，
 * 子进程用 execl 方式启动 more 程序，那么 more 程序和子进程共享文件描述符（这是存储模型的一部分），
 * 所以子进程把管道读端映射到标准输入文件，就可以将父进程的数据直接定向到 more，完成分页；
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


#define PAGER   "/bin/more"     // 既有分页程序

#define N       (8)             // fgets 的缓冲区最大长度，可以小于一行的长度，那就一行读多次行了


int main(int argc, char * argv[])
{
    // 参数检查
    if (argc != 2)
    {
        printf("usage: %s filename\n", argv[0]);exit(1);
    }

    FILE    *fp;
    int     fd[2];
    pid_t   pid;
    char    buf[N];
    int     n;
    char    *fn, *sn;

    // 打开欲显示文件
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        printf("file open failed...\n");exit(2);
    }

    // 创建 pipe，以传递待显示文件内容
    if (pipe(fd) < 0)
    {
        printf("pipe creatation failed...\n");exit(3);
    }

    // 创建子进程，以启动分页程序
    if ((pid = fork()) < 0)             // 创建子进程失败
    {
        printf("fork failed...\n");exit(4);
    }
    else if (pid > 0)                   // 父进程
    {
        close(fd[0]);   // 父进程不用读

        /*
         * 读文件内容，通过管道给分页程序
         * 一次读一行（肯定是文本），
         * 如果缓冲大小大于当前行长，则读完这一行，
         * 如果缓冲大小小于当前行长，则这一行多次读完，
         * 回车换行算一个字符
         */
        while (fgets(buf, N, fp) != NULL)
        {
            n = strlen(buf);
            if (write(fd[1], buf, n) != n)
            {
                printf("write to pipe failed...\n");exit(5);
            }
        }
/*        if (ferror(fp))   // 这个先不加
        {
            printf("fgets error");              exit(6);
        }//*/

        // 文件传送完毕，关闭pipe写端
        close(fd[1]);

        /* Note:
         * 等待子进程执行结束，
         * 如果没有这一句，或者有这一句，但参数2（0起）使用 WNOHANG，
         * 将使得父进程不用等待子进程结束即可自行结束，由于父进程的退出，将导致子进程的退出，最终导致分页不能连续完整执行；
         * 必须使用这一句，等待特定的子进程，并阻塞
         */
        waitpid(pid, NULL, 0);

        exit(0);
    }
    else                                // 子进程
    {
        // 子进程不需要写
        close(fd[1]);

        // 将管道读端映射到标准输入
        if (fd[0] != STDIN_FILENO)  // 判断一下管道读端是不是已经映射到标准输入，如果已经映射到还dup2，则会在之后将其关掉，这是不对的
        {
            if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
            {
                printf("dup to stdin failed...\n");exit(7);
            }

            close(fd[0]);   // 不再需要这个pipe读端描述符，以后使用标准输入作为pipe读端描述符
        }

        // 取分页程序的全名、短名，作为execl的参数
        fn = PAGER;
        if ((sn = strrchr(fn, '/')) != NULL)
            sn ++;
        else
            sn = fn;

        // 执行分页程序：这个程序从标准输入拿到数据，就是管道的读端，就是父进程发过来的文件内容
        if (execl(fn, sn, (char *)0) < 0)
        {
            printf("execl failed...\n");exit(8);
        }
    }

    exit(0);
}

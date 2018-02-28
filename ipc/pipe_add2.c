#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


#define N       (1024)


int main(void)
{
    int n, fd1[2], fd2[2];  // 1-向协同进程发送，2-从协同进程接收
    pid_t pid;
    char buf[N];

    // 捕捉 SIG_PIPE 信号

    if ((pipe(fd1) < 0) || (pipe(fd2) < 0))
    {
        printf("pipe created failed...\n");exit(1);
    }

    if ((pid = fork()) < 0)
    {
        printf("fork failed...\n");exit(2);
    }
    else if (pid > 0)
    {
        close(fd1[0]);
        close(fd2[1]);

        while (fgets(buf, N, stdin) != NULL)    // 直接传送整型类型，并接收整型返回值
        {
            int a[2];
            sscanf(buf, "%d%d", a, a + 1);
            write(fd1[1], (unsigned char *)a, 8);
            int b;
            n = read(fd2[0], &b, N);
            printf("%d\n", b);
        }
#if 0
        while (fgets(buf, N, stdin) != NULL)    // 传送自己从终端接收到的文本数据，接收协同进程返回的文本格式数据
        {
            n = strlen(buf);
            write(fd1[1], buf, n);
            n = read(fd2[0], buf, N);
            buf[n] = 0;
            fputs(buf, stdout);
        }
#endif
    }
    else
    {
        close(fd1[1]);
        close(fd2[0]);

        dup2(fd1[0], STDIN_FILENO);
        close(fd1[0]);
        dup2(fd2[1], STDOUT_FILENO);
        close(fd2[1]);

#ifdef ADD_READ
        execl("./add2", "add2", (char *)0);
#endif

#ifdef ADD_FGETS
        execl("./add2_stdio", "add2_stdio", (char *)0);
#endif
    }

    exit(0);
}

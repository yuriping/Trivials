#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int fd[2];
    pid_t pid;
    int n;
    char buf[1024] = {0};

    if (pipe(fd) < 0)   // return 0 if succeed, otherwise -1
    {
        printf("pipe created failed...\n");
        exit(1);
    }

    if ((pid = fork()) < 0)
        printf("fork error\n");
    else if (pid > 0)
    {
        close(fd[0]);
        write(fd[1], "hello world\n", 12);
    }
    else
    {
        close(fd[1]);
        n = read(fd[0], buf, 1024);
        write(STDOUT_FILENO, buf, n);
    }

    exit(0);
}

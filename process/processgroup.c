#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main(void)
{
    pid_t parentid, childid;

    parentid = getpid();
    printf("Parent id = %d\n", parentid);

    pid_t pid;

    pid = fork();

    if (pid < 0)
    {
        printf("%s\n", "fork error");
        exit(1);
    }

    if (0 == pid)
    {
        printf("%s, parentid stored = %d, parentid get = %d, my id = %d\n", "i am child", parentid, getppid(), getpid());

        printf("child setpgid : %d\n", setpgid(0, getppid()/*getppid()*/));

        printf("child group id = %d, %d\n", getpgid(0), getpgrp());

        printf("child set s id : %d\n", setsid());

        printf("child group id = %d\n", getpgid(0));

        printf("child get sid = %d\n", getsid(0));
    }
    else
    {
        printf("%s, my id = %d, my child id = %d\n", "i am parent", getpid(), pid);

        printf("parent setpgid : %d\n", setpgid(0, 0/*pid()*/));

        printf("parent group id = %d, %d\n", getpgid(0), getpgrp());

        printf("parent set s id : %d\n", setsid());

        printf("parent group id = %d\n", getpgid(0));

        printf("parent get sid = %d\n", getsid(0));

        printf("parent child sid = %d\n", getsid(/*3333*/pid));
    }

    sleep(1);
    exit(0);
}

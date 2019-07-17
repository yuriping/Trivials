
/*
 * 只有子进程退出的时候，父进程依然在运行，才能捕捉到 SIGCHLD 信号，否则，将不会捕捉到。
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


typedef void SIG_HNDL_T(int);

void SigHndl_SIGCHLD_Zombie(int signum)
{
    signum = signum;

    pid_t   pid;
    int     statloc;

    while ((pid = waitpid(-1, &statloc, WNOHANG)) > 0)
    {
        printf("child %d terminated\n", pid);
    }

    return;
}

SIG_HNDL_T * SigHndlInit(int signum, SIG_HNDL_T * handler)
{
    struct sigaction act, oact;

    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (SIGALRM == signum)
    {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    }
    else
    {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }

    if (sigaction(signum, &act, &oact) < 0)
        return SIG_ERR;

    return oact.sa_handler;
}

int main(void)
{
    if (SIG_ERR == SigHndlInit(SIGCHLD, SigHndl_SIGCHLD_Zombie))
    {
        fprintf(stderr, "Signal Initiation Failed\n");
        exit(1);
    }

    pid_t pid;

    if ((pid = fork()) < 0) // error...
    {
        printf("fork error...\n");  exit(1);
    }
    else if (pid == 0)      // child
    {
        printf("child...\n");
//        sleep(2);
        printf("child exit...\n");
//        exit(2);
    }
    else                    // parent
    {
        printf("parent...\n");
        sleep(5);  // NOTE : 这个 sleep 会在子进程退出时，因收到信号，被提前结束，即使她的延时比子进程中延时长
        printf("parent exit...\n");
    }

//    sleep(3);
    printf("main exit...\n");

/*
 * 不用这个无限循环，并使子进程在上述括号内退出，也可以，
 * 只要使主进程中的时间比子进程长就行，保证子进程退出时，主进程没有退出，并能收到信号。
 */
//    for (;;)
//        sleep(1);

    exit(0);
}

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void sigHdl_SIGINT(int signum)
{
    printf("sig captured...\n");

    int i, j;
    volatile int k;

    printf("%s\n", "delay start...");
    for (i = 0; i< 300000; i ++)
        for (j = 0; j < 4000; j ++)
            k += i * j;
    printf("%s\n", "delay end...");

    return;
}

void * mysignal(int signum, void * handler)
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
//        act.sa_flags |= SA_RESETHAND;
//        act.sa_flags |= SA_NODEFER;
    }

    if (sigaction(signum, &act, &oact) < 0)
        return SIG_ERR;

    return oact.sa_handler;
}

int main(int argc, char const *argv[])
{
    mysignal(SIGINT, sigHdl_SIGINT);

    while(1);

    exit(0);
}

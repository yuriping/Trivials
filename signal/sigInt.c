
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef void SIG_HNDL_T(int);

void SigHndl_SIGINT_Prnt(int signum)
{
    (void)signum;

    printf("SigHandler_SIGCHLD_Zombie\n");

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
    if (SIG_ERR == SigHndlInit(SIGINT, SigHndl_SIGINT_Prnt))
    {
        fprintf(stderr, "Signal Initiation Failed\n");
        exit(1);
    }

    int i = 0;

    for (;;)
    {
        printf("%d\n", i ++);

        sleep(1);
    }

    exit(0);
}

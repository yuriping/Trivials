#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf env_alrm;

static void sig_alrm(int signo)
{
    longjmp(env_alrm, 1);
}

unsigned int sleep2(unsigned int nsecs)
{
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return nsecs;

    if (setjmp(env_alrm) == 0)
    {
        alarm(nsecs);
        pause();
    }

    return alarm(0);
}

int main(void)
{
    printf("Start...\n");

    int remain_secs = sleep2(5);

    printf("End... %d\n", remain_secs);

    exit(0);
}

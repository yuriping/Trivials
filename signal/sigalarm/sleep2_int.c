#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf env_alrm;

static void sig_alrm(int signo)
{
    siglongjmp(env_alrm, 1);
}

static void sig_int(int signo)
{
    int i, j;
    volatile int k;

    printf("\nINT signal Start...\n");

    for (i = 0; i < 300000; i ++)
    {
        for (j = 0; j < 4000; j ++)
        {
            k += i * j;
        }
    }

    printf("\nINT signal End...\n");
}

unsigned int sleep2(unsigned int nsecs)
{
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return nsecs;

    if (sigsetjmp(env_alrm, 1) == 0)
    {
        alarm(nsecs);
        pause();
    }

    return alarm(0);
}

int main(void)
{
    if (signal(SIGINT, sig_int) == SIG_ERR)
    {
        printf("%s\n", "sig int error");
    }


    printf("Alarm Test Start...\n");

    int remain_secs = sleep2(3);

    printf("Alarm Test End... remain secs = %d\n", remain_secs);

    exit(0);
}

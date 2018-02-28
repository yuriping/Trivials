#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>


static sigjmp_buf               jmpbuf;

static volatile sig_atomic_t    jmpready;


void pr_mask(const char *str)
{
    sigset_t    sigset;

    if (sigprocmask(0, NULL, &sigset) < 0)
    {
        printf("%s\n", "sigprocmask error");
        exit(1);
    }

    printf("%s", str);
    if (sigismember(&sigset, SIGINT))   printf("SIGINT ");
    if (sigismember(&sigset, SIGQUIT))  printf("SIGQUIT ");
    if (sigismember(&sigset, SIGUSR1))  printf("SIGUSR1 ");
    if (sigismember(&sigset, SIGALRM))  printf("SIGALRM ");

    /* remaining signals can go here  */

    printf("\n");
}

void sighdl_SIGALRM(int signum)
{
    pr_mask("in sig_alrm: ");

    return;
}

void sigHdl_SIGUSR1(int signum)
{
    pr_mask("sig usr1 captured...");

    alarm(3);

    int i, j;
    volatile int k;

    printf("%s\n", "delay start...");
    for (i = 0; i< 300000; i ++)
        for (j = 0; j < 4000; j ++)
            k += i * j;
    printf("%s\n", "delay end...");

    pr_mask("sig usr1 finished ...");

    if (jmpready)
    {
        siglongjmp(jmpbuf, 1);
    }
    else
    {
        printf("%s\n", "jmp point not ready yet");
    }

    return;
}

int main(int argc, char const *argv[])
{
    signal(SIGUSR1, sigHdl_SIGUSR1);

    signal(SIGALRM, sighdl_SIGALRM);

    pr_mask("main start...");

    if (sigsetjmp(jmpbuf, 1/*0*/))
    {
        pr_mask("main end ...");
        exit(0);
    }
    else
    {
        jmpready = 1;
    }

    while(1)
        pause();

    exit(0);
}

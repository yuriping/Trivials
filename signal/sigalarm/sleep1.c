#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


static void sig_alrm(int signo)
{
    // nothing to be...
}

unsigned int sleep1(unsigned int nsecs)
{
    if (SIG_ERR == signal(SIGALRM, sig_alrm))
    {
        return nsecs;
    }

    alarm(nsecs);

    pause();

    return (alarm(0));
}


int main(void)
{
    printf("Start...\n");

    int a = sleep1(5);

    printf("End...%d\n", a);

    exit(0);
}


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sig_alrm(int signo)
{
    //
}

unsigned int sleep1(unsigned int nsecs)
{
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
        return (nsecs);

    alarm(nsecs);

    pause();

    return (alarm(0));
}

int main(void)
{
    printf("start...\n");

    sleep1(3);

    printf("end...\n");

    exit(0);
}

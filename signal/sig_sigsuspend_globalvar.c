#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


volatile sig_atomic_t   quitflag;


static void sig_hdl(int signo)
{
//    printf("%d\n", signo);
    if (SIGINT == signo)
        printf("%s\n", "sig int");
    else if(SIGQUIT == signo)
    {
        printf("%s\n", "sig quit");
        quitflag = 1;
    }
}

int main(int argc, char const *argv[])
{
    quitflag = 0;

    signal(SIGINT, sig_hdl);
    signal(SIGQUIT, sig_hdl);

    sigset_t newmask, oldmask, zeromask;

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);

    sigprocmask(SIG_BLOCK, &newmask, &oldmask);

    while (0 == quitflag)
        sigsuspend(&zeromask);

    quitflag = 0;

    sigprocmask(SIG_SETMASK, &oldmask, NULL);

    exit(0);
}

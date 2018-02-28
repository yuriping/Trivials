#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    long clktck = 0;
    clktck = sysconf(_SC_CLK_TCK);

    printf("tick of one second : %ld\n", clktck);

    exit(0);
}

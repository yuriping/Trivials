#include "main.h"
#include <stdio.h>
#include "Thread.h"

THREAD thr0;

int main(void)
{
    ThreadStart();

    unsigned char time = 0;
    while (time < 5)
    {
	time ++;
        sleep(1);
    }

    ThreadStop();

    while (1);

    return 0;
}



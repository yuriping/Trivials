#include <stdio.h>

int buf_sum(int * buf, int count)
{
    int sum = 0;
    int i;
    for (i=0; i< count; i++)
    {
        sum += *(buf + i);
    }

    return sum;
}


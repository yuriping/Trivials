#include "./main.h"
#include <stdio.h>


void Output(int a, int b, CbFun_T func)
{
    printf("a = %d, b = %d\n", a, b);

    int c = func(a, b);

    printf("c = %d\n", c);

    return;
}

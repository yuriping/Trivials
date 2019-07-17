#include "./main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int calc(int a, int b)
{
    return a + b;
}


void Output(int, int, CbFun_T);

int main()
{
    Output(1, 2, calc);

    exit(0);
}

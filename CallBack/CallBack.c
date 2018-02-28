#include <stdio.h>

signed int calculate(signed int a, signed int b)
{
    return a + b;
}

void caller(void)
{
    
    printf("%d\n", callee(calculate, 1, 2));
}

signed int callee(signed int (*CbFxn)(signed int a, signed int b), signed int a, signed int b)
{
   return CbFxn(a,b);
}

int main(void)
{

    caller();

    return 0;
}



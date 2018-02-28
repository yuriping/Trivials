#include <stdio.h>

typedef struct FUNCPTR {
    unsigned int (* pFunAdd) (unsigned int a, unsigned int b);
    unsigned int a;
    unsigned int b;

//    unsigned int FunAdd2(unsigned int a, unsigned int b);
} FUNCPTR;

unsigned int Add2 (unsigned int a, unsigned int b)
{
    return a + b;
}

int main(void)
{
    FUNCPTR fun;

    fun.a = 1;
    fun.b = 2;
    fun.pFunAdd = Add2;

    unsigned int c = fun.pFunAdd(fun.a, fun.b);

    printf("%d\n", c);

    return 0;
}



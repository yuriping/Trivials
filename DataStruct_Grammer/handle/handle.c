#include <stdio.h>

typedef struct
{
    unsigned char ch;
    unsigned int  i;
} STRUCT, * HANDLE;

int main(void)
{
    STRUCT s;
    s.ch = 'y';
    s.i  = 100;

    HANDLE ps = &s;

    printf("\n%c\n%d\n", ps->ch, ps->i);

    return 0;
}


#include <stdio.h>

typedef struct
{
    unsigned int id;

    union
    {
        unsigned char ch;
        float         f;
    } u;
} STRUCT, * P_STRUCT;

int main(void)
{
    STRUCT s0, s1;

    s0.id = 0;
    s0.u.ch = 'a';

    s1.id = 1;
    s1.u.f = 12.3;

    printf("s0: %c - %f\n", s0.u.ch, s0.u.f);
    printf("s1: %c - %f\n", s1.u.ch, s1.u.f);
}


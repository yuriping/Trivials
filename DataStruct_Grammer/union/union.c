#include <stdio.h>

typedef union
{
    char  ch;
    int   i;
    float f;
} UNION;

int main(void)
{
    UNION u;

    u.ch = 'a';
    printf("char : %c\n", u.ch);

    u.i = 100;
    printf("integer : %d\n", u.i);

    //printf("char : %c\n", u.ch);

    u.f = 15.6;
    printf("float : %f\n", u.f);

    return 0;
}

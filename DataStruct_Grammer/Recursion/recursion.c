#include <stdio.h>

typedef struct STRUCT0
{
    unsigned char ch;
    struct STRUCT1 * pstru;
} STRUCT0, * P_STRUCT0;

typedef struct STRUCT1
{
    unsigned int i;
    struct STRUCT0 * pstru;
} STRUCT1, * P_STRUCT1;

int main(void)
{
    STRUCT0 stru0;
    STRUCT1 stru1;

    stru0.ch = 'a';
    stru0.pstru = &stru1;

    stru1.i = 12;
    stru1.pstru = &stru0;

    printf("stru0: %c\n", stru0.ch);
    printf("stru1: %d\n", stru1.i);

    printf("=========================\n");

    printf("stru0->stru1: %d\n", (stru0.pstru)->i);
    printf("stru1->stru0: %c\n", (stru1.pstru)->ch);

    printf("=========================\n");

    P_STRUCT0 pstru0 = &stru0;
    P_STRUCT1 pstru1 = &stru1;

    printf("pstru0->: %c\n", pstru0->ch);
    printf("pstru1->: %d\n", pstru1->i);

    return 0;
}


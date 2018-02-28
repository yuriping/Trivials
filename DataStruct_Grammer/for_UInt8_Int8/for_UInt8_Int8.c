#include <stdio.h>

int main(void)
{
    unsigned char uch = 0;
    signed char   ch  = 0;
    signed char   prev = uch - 1;

    printf("\n%d\n", prev);
    for (ch = 0; ch < prev; ch++)
    {
        printf("%5d", ch);
    }

    printf("\n");

    return 0;
}

#include <stdio.h>

int main(void)
{
    signed char chch;
    unsigned char uchuch;

    chch = -1;
    uchuch = -1;
    printf("ch = %d, uch = %d\n", chch, uchuch);
    chch = 255;
    uchuch = 255;
    printf("ch = %d, uch = %d\n", chch, uchuch);

    return 0;
}



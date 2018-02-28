#include <stdio.h>
#include <stdlib.h>

#define LEN	10

void fun(char ** ptr_int)
{
    char * p_int = (char *)malloc(10);

    unsigned char index;
    for (index = 0; index < LEN; index ++)
    {
        *(p_int + index) = index + 3;
    }

    * ptr_int = p_int;
}

int main(void)
{
    char * pointer_int;

    fun(&pointer_int);

    unsigned char i;
    for (i = 0; i < LEN; i ++)
    {
        printf("%3d    ", (int)(*(pointer_int + i)));
    }

    printf("\n");

    return 0;
}
															

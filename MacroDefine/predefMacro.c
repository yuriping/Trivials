#include <stdio.h>

int main(void)
{
    printf("File : %s, Line = %d\n", __FILE__, __LINE__);

    printf("Date : %s, Time : %s\n", __DATE__, __TIME__);

    unsigned char ch = 'c';
    unsigned int  i  = 10;

    printf("%s:[%d] ch = %c, i = %d\n", __FILE__, __LINE__, ch, i);

    return 0;
}


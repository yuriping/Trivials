#include <stdio.h>

#define paster(n) printf("token"#n" = %d\n ", token##n) 

int main(void)
{
    unsigned char token9 = 7;

    paster(9);

    return 0;
}


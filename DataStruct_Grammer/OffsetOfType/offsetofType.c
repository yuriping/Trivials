#include <stdio.h>

int main(void)
{
    typedef struct ABC
    {
        unsigned char ch;
        unsigned int  i;
        unsigned char ch2;
    } __attribute__((packed)) ABC;

#define offsetof(TYPE, FLD) ((size_t)(&(((TYPE *)0)->FLD)))

    printf("offsetof ch = %d\n", offsetof(ABC, ch));
    printf("offsetof i  = %d\n", offsetof(ABC, i));
    printf("offsetof ch2= %d\n", offsetof(ABC, ch2));

    return 0;
}


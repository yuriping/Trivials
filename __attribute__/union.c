#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    unsigned short idx;
    unsigned char * pch;

    typedef struct SUB_T
    {
        char sc;
        int ss;
    } /* __attribute__((packed)) */ SUB_T;

    typedef union UNION_UNPACKED
    {
        char  c;
        short s;
        int   i;
//        SUB_T st;
    } UNION_UNPACKED;

    typedef union UNION_PACKED
    {
        char  c;
        short s;
        int   i;
//        SUB_T st;
    } __attribute__((packed)) UNION_PACKED;

#define LX  UNION_PACKED

    LX u0, u1;
    memset(&u0, 0x55, sizeof(LX));
    memset(&u1, 0xee, sizeof(LX));
    u0.c = 0x32;
    u1.s = 0x1234;

    for (pch = (unsigned char *)&u0, idx = 0; idx < sizeof(LX); idx ++)
        printf("%02x-", pch[idx]);
    printf("\n");
    for (pch = (unsigned char *)&u1, idx = 0; idx < sizeof(LX); idx ++)
        printf("%02x-", pch[idx]);
    printf("\n");

//    memset()

    printf("sub_t = %ld, unpacked = %ld, packed = %ld\n", sizeof(SUB_T), sizeof(UNION_UNPACKED), sizeof(UNION_PACKED));

    exit(0);
}


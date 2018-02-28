
#include <stdio.h>

#define PUT_LOG(fp, ...)	fprintf(fp, __VA_ARGS__);


int main(void)
{
    FILE * fp;

    fp = fopen("file", "a+");
    PUT_LOG(fp, "%d\n", 12);

    return 0;
}



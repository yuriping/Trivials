#include <stdio.h>
#include <stdlib.h>


#define N       (1024)

int main(void)
{
    int n, int1, int2;
    char buf[N];

//    setvbuf(stdin, NULL, _IOLBF, 0);
//    setvbuf(stdout, NULL, _IOLBF, 0);

    while (fgets(buf, N, stdin) != NULL)
    {
        if (sscanf(buf, "%d%d", &int1, &int2) == 2)
        {
            printf("%d\n", int1 + int2);
        }
        else
        {
            printf("int error...\n");
            continue;
        }
    }

    exit(0);
}

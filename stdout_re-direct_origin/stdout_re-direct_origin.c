#include <stdio.h>

int main(void)
{
    char buf[10];
    char idx;
    for (idx = 0; idx < 10; idx ++)
    {
        buf[idx] = idx + 1;
    }
//    buf[11] = 0;

//    fprintf(stdout, "%s", buf);
    for (idx = 0; idx < 10; idx ++)
    {
        fprintf(stdout, "%c", buf[idx]);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    if (system(NULL))
    {
        printf("system is available.\n");
    }
    else
    {
        printf("system is nonsense\n");
    }

    exit(0);
}

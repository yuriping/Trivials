#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int main(void)
{
    system("cp 0_0* ./dir");
    printf("completed...\n");

    return 0;
}



#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char * filename = "a";

    if (access(filename, F_OK))
    {
        printf("file no exit!\n");
    }
    else
    {
        printf("file exit...\n");
        if (remove(filename))
            printf("cannot be removed...\n");
        else
            printf("file has been removed...\n");
    }

    return 0;
}



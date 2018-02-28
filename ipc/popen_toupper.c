#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(void)
{
    char buf[1024];
    FILE * fin;

    fin = popen("./toupper", "r");

    while (1)
    {
        fputs("prompt > ", stdout);        fflush(stdout);

        if ((fgets(buf, 1024, fin)) == NULL)
            break;

        fputs(buf, stdout);
    }

    pclose(fin);

    exit(0);
}

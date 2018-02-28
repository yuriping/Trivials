#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    char ch;

    while ((ch = getchar()) != EOF)
    {
        if (islower(ch))
            ch = toupper(ch);

        putchar(ch);

        if ('\n' == ch)
            fflush(stdout);
    }

    exit(0);
}

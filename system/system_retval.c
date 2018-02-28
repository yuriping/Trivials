#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    int status;

    status = system("date");
    printf("%d\n", status);
    printf("========\n");

    status = system("nosuchcommand");
    printf("%d\n", status);
    printf("========\n");

    status = system("who; exit 44");
    printf("%d\n", status);
    printf("========\n");

    status = system("ls adfad");
    printf("%d\n", status);
    printf("========\n");
}

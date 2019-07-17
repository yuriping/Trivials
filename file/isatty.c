#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    int fd;
    int ret;

    ret = isatty(STDIN_FILENO);
    printf("is a tty : STDIN_FILENO = %d\n", ret);

    fd = open("./1", O_RDONLY | O_CREAT, 0);
    ret = isatty(fd);
    printf("is a tty : 1 = %d\n", ret);
    close(fd);

    exit(0);
}

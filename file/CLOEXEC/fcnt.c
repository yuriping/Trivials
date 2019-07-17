#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
    int fd = open("./doc.txt", O_RDWR | O_CREAT);
    unsigned int flag = fcntl(fd, F_GETFD);
    flag |= ~FD_CLOEXEC; //去掉'~'看看
    fcntl(fd, F_SETFD, flag);

    char f[20];
    sprintf(f, "%d", fd);
    printf("[fcnt] fd = %s\n", f);

    int ret = execl("./test", f, (char *)0);
    printf("ret: %d\n", ret);

    return 0;
}


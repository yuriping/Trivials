#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
//#include <sys/stat.h>
//#include <sys/types.h>
#include <unistd.h>


int main(void)
{
    /* dup */
    int fd = dup(1);
//  close(fd);
    write(fd, "yuriping\n", 9);//*/

    /* dup2 */
    int fd1;
    fd1 = open("file1", O_WRONLY | O_CREAT);
    printf("fd1 = %d\n", fd1);
    write(fd1, "abcde\n", 6);

    int fdr = dup(fd1);

    dup2(1, fd1);
    printf("fd1 = %d\n", fd1);
//    close(4);
    write(fd1, "hello world\n", 12);

    dup2(fdr, fd1);
    write(fd1, "huifule\n", 8);

    exit(0);
}

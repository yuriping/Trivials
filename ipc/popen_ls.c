/*
 * 演示功能：
 * popen 的读打开工作方式
 */

#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

int main(void)
{
    FILE * fpout = popen("ls", "r");

    char buf[1024]; bzero(buf, 1024);

    int len = fread(buf, 1024, 1, fpout);

    printf("len = %d, %ld\n", len, strlen(buf));
    printf("%s\n", buf);

    if (strstr(buf, ".c"))
        printf("have\n");
    else
        printf("do not have\n");

    pclose(fpout);

    return 0;
}

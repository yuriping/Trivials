#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char ** argv)
{
    printf("========[test]========\n");
    printf("argc = %d\n", argc);
    unsigned char idx;
    for (idx = 0; idx < argc; idx ++)
        printf("argv[%d] = %s\n", idx, argv[idx]);

    int fd = atoi(argv[0]);

    char str[16];
    strcpy(str, "Hello World!\n");
    printf("%s\n", str);

    int ret = write(fd, str, strlen(str));
    printf("ret: %d\n", ret);

    return 0;
}


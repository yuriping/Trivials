#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define N       (1024)


int main(void)  // 调用者传来的整型数据，并以整型返回结果
{
    // Note:这个函数里调用打印不会输出，因为打印被重定向到了管道，要打印请向标准错误打印
    int n, int1, int2;
    char buf[N];

    while ((n = read(STDIN_FILENO, buf, N)) > 0)
    // 即使是这种read，也必须有个换行，否则不知道文件到了结尾，所以直接传送二进制值不行，要改造
    {
        buf[n] = 0;
        int res = *((int *)(buf)) + *((int *)(buf + 4));
        write(STDOUT_FILENO, &res, 4);
    }

    exit(0);
}

#if 0
int main(void)  // 接收调用者传来的文本数据，并以文本返回数值
{
    int n, int1, int2;
    char buf[N];

    while ((n = read(STDIN_FILENO, buf, N)) > 0)
    // 即使是这种read，也必须有个换行，否则不知道文件到了结尾，所以直接传送二进制值不行，要改造
    {
        buf[n] = 0;
        if (sscanf(buf, "%d%d", &int1, &int2) == 2)
        {
            sprintf(buf, "%d\n", int1 + int2);
            n = strlen(buf);
            write(STDOUT_FILENO, buf, n);
        }
        else
        {
            printf("int error...\n");
            continue;
        }
    }

    exit(0);
}
#endif

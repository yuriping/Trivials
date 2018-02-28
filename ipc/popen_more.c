/*
 * 演示功能：
 * 使用 popen 实现向 既有分页程序 more 传送文件
 *
 * 工作原理：
 * 使用popen执行more，作为写方式打开，返回文件描述符fpout，将文件的内容再写入fpout即可；
 */

#include <stdio.h>
#include <stdlib.h>


#define PAGER   "/bin/more"

#define N           (1024)


int main(int argc, char * argv[])
{
    // 检查参数
    if (argc != 2)
    {
        printf("usage: %s filename\n", argv[0]);exit(1);
    }

    char buf[N];
    FILE *fpin, *fpout;

    // 打开要显示的文件
    if ((fpin = fopen(argv[1], "r")) == NULL)
    {
        printf("file %s open failed...\n", argv[1]);exit(2);
    }

    // 打开分页程序，写方式
    if ((fpout = popen(PAGER, "w")) == NULL)
    {
        printf("popen failed...\n");exit(3);
    }

    /* 向分页程序发送文件内容 */
    while (fgets(buf, N, fpin) != NULL)
    {
        if (fputs(buf, fpout) == EOF)
        {
            printf("fputs failed...\n");exit(4);
        }
    }

    /*
    * 如果不关闭，一屏能显示完的短文件，也要按回车才能回到Shell提示符；
    * 多屏显示的长文件，也不能多屏显示了，只能显示一屏内容，之后的退出
     */
    if (pclose(fpout) == -1)
    {
        printf("pclose failed...\n");exit(5);
    }//*/

    exit(0);
}

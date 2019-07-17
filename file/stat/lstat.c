#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>       // lstat

int main(int argc, char * argv[])
{
    int idx;            // 遍历 main 传入参数
    struct stat buf;    // 存 lstat 结果
    char * msg;         // 用于显示文件类型的字符串指针

    /* 遍历 main 传入参数，每个参数是一个文件名，用于查询其类型 */
    for (idx = 1; idx < argc; idx ++)   // 从 1 开始才是参数，0 是程序本身
    {
        printf("%s : ", argv[idx]);

        if (lstat(argv[idx], &buf) < 0) // 如出错，一般是文件不存在
        // 此时符号链接的类型就是 符号链接，如果用stat、fstat，则符号链接的类型是 regular，lstat 不跟随链接，其他的跟随链接到其指向的文件
        {
            printf("stat error\n");     continue;
        }

        /* 根据 st_mode 得到文件类型，用宏测试 */
        if      (S_ISREG(buf.st_mode))
            msg = "regular";
        else if (S_ISDIR(buf.st_mode))
            msg = "directory";
        else if (S_ISCHR(buf.st_mode))
            msg = "character special";
        else if (S_ISBLK(buf.st_mode))
            msg = "block special";
        else if (S_ISFIFO(buf.st_mode))
            msg = "fifo";
        else if (S_ISLNK(buf.st_mode))
            msg = "symbolic link";
        else if (S_ISSOCK(buf.st_mode))
            msg = "socket";
        else
            msg = "unknown mode";

        printf("%s\n", msg);
    }

    exit(0);
}

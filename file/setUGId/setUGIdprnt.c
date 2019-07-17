/*
 * 书中 4.4
 * 演示如何查看文件是否设置了：设置用户 ID 位，与 设置组 ID 位
 *
 * NOTE :
 *     0. S_ISUID、S_ISGID 是变量宏，不是函数宏，使用按位与测试，结果 0 就是未设置，非 0 就是设置
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


/*
 * 简化程序判断，Usage：./setUGIdprnt filename （仅检测一个文件）
 */
int main(int argc, char * argv[])
{
    struct stat buf;    // 存 lstat 结果
    char * msg;         // 用于显示测试结果的字符串指针

    /* 执行 lstat，如果是软链接，不跟随 */
    if (lstat(argv[1], &buf) < 0)   // 如出错，一般是文件不存在
    {
        printf("stat error...\n");      exit(1);
    }

    /* 打印一些信息 */
    // st_mode 的大小 = 4 Bytes，在 64 位系统有 Warning，不管
    printf("size of mode_t : %d\n", sizeof(buf.st_mode));
    // S_ISUID、S_ISGID 是变量宏，不是函数宏，显示他们的值 S_ISUID = 0x00000800, S_ISGID = 0x00000400
    printf("S_ISUID = 0x%08x, S_ISGID = 0x%08x\n", S_ISUID, S_ISGID);

    /* 检测是否设置用户 ID */
    if (S_ISUID & buf.st_mode)
        msg = "set user id";
    else
        msg = "not set user id";
    printf("%s\n", msg);

    /* 检测是否设置组 ID */
    if (S_ISGID & buf.st_mode)
        msg = "set group id";
    else
        msg = "not set group id";
    printf("%s\n", msg);

    exit(0);
}

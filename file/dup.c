/*
 * 文件描述符的复制函数 dup、dup2 的演示
 */


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    /*
     * dup 函数
     */
    /* 标准输出描述符，复制到一个新的文件描述符，当前可用最小的 */
    int fd = dup(1);
    printf("old fd = %d, dup : new fd = %d\n", 1, fd);
    // 写到新描述符，等于写到标准输出
    write(fd, "yurp@eastsoft.com.cn\n", sizeof("yurp@eastsoft.com.cn\n"));

    printf("----------------------------------------------------------------\n");

    /*
     * dup2 函数
     */
    // 打开一个磁盘文件，写，如不存在创建
    int fd1 = open("file1", O_WRONLY | O_CREAT);
    printf("fd1 = %d\n", fd1);
    write(fd1, "abcde\n", sizeof("abcde\n"));

    /* 演示 fd1 给 标准输出用，之后再恢复到 file1 上 */
    // 暂存 file1 的描述符
    int fdr = dup(fd1);
    printf("fdr = %d\n", fdr);
    // 把标准输出的描述符赋予原 file1 的描述符，并执行操作，向标准输出
    dup2(1, fd1);
    printf("fd1 as stdin = %d\n", fd1);
    write(fd1, "hello world\n", sizeof("hello world\n"));
    // 恢复 file1 的描述符到 fd1
    dup2(fdr, fd1);
    write(fd1, "huifule\n", sizeof("huifule\n"));

    exit(0);
}

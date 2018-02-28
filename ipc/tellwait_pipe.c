/*
 * 演示功能：
 * 使用匿名管道实现父子进程之间的同步，即执行先后顺序设置
 *
 * 工作原理：
 * 父子进程创建后的执行顺序是随机的，如果要保证其执行的确定顺序，就必须做同步处理；
 * 这里采用pipe方式，只有父进程向子进程发送一个字符，子进程才认为收到同步信号；反之亦然；
 * 设立两个pipe，1号父进程向子进程发送同步信令（字符‘p’），2号子进程向父进程发送同步信令（字符‘c’）
 * Note:本例中，不用同步操作，也不会出现字符交叉输出的问题，这可能是时间片轮转的原因，用原书李程验证也是这样……
 *      各个同步函数是有返回值的，但为简便易见，都没有做判断……
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


static int fd1[2], fd2[2];  // 1：父写子读，2：父读子写


/* 逐个字符地输出字符串，以0x00为结束符 */
static void CharsPut(char * str)
{
    // 设置标准输出为无缓冲，否则，必须到换行才能输出显示，达不到测试效果
    setbuf(stdout, NULL);

    char * ptr;
    char c;
    for (ptr = str; (c = *ptr ++) != 0;)
        putc(c, stdout);

    return;
}

/* 同步量初始化 */
bool TellWaitInit(void)
{
    if ((pipe(fd1) < 0) || (pipe(fd2) < 0))
    {
        printf("init failed...\n");
        return false;               // 同步量初始化失败
    }

    return true;                    // 同步量初始化成功
}

/* 释放父进程 */
bool TellParent(void)
{
    if (write(fd2[1], "c", 1) != 1)
    {
        printf("TellParent failed...\n");
        return false;               // 释放父进程失败
    }

    return true;                    // 释放父进程成功
}

/* 等待父进程 */
bool WaitParent(void)
{
    char c = 0x00;
    int len = read(fd1[0], &c, 1);

    if ((len != 1) || (c != 'p'))
    {
        printf("WaitParent failed...\n");
        return false;               // 等待父进程失败
    }

    return true;                    // 等待父进程成功
}

/* 释放子进程 */
bool TellChild(void)
{
    if (write(fd1[1], "p", 1) != 1)
    {
        printf("TellChild failed...\n");
        return false;               // 释放子进程失败
    }

    return true;                    // 释放子进程成功
}

/* 等待子进程 */
bool WaitChild(void)
{
    char c = 0x00;
    int len = read(fd2[0], &c, 1);

    if ((len != 1) || (c != 'c'))
    {
        printf("WaitChild failed...\n");
        return false;               // 等待子进程失败
    }

    return true;                    // 等待子进程成功
}

int main(void)
{
    pid_t pid;

    TellWaitInit();

    if ((pid = fork()) < 0)
    {
        printf("fork failed...\n");exit(1);
    }
    else if (pid > 0)   // Parent
    {
        //WaitChild();
        CharsPut("parent\n");
        TellChild();
    }
    else                // Child
    {
        WaitParent();
        CharsPut("child\n");
        //TellParent();
    }

    exit(0);
}

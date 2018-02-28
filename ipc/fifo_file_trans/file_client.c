#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXLINE 1024
#define SERV_FIFO "./fifo.serv"
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
  
int main(int argc, char** argv)
{
    int     readfifo;
    int     writefifo;
    size_t  len;
    ssize_t n;
    char*   ptr;
    char    fifoname[MAXLINE];
    char    buff[MAXLINE];
    pid_t   pid;

    //创建自己的FIFO 用于接收服务器端的数据
    pid = getpid();
    snprintf(fifoname, sizeof(fifoname), "./fifo.%ld", (long)pid);
    if (mkfifo(fifoname, FILE_MODE) < 0 && (errno != EEXIST))
        printf("can't create %s", fifoname);

    //构造请求消息
    snprintf(buff, sizeof(buff), "%ld ", (long)pid);
    len = strlen(buff);
    ptr = buff + len ;  
    fgets(ptr, MAXLINE - len, stdin); //从键盘获取请求文件路径
    len = strlen(buff);

    //打开服务器的FIFO 向其中写入消息
    writefifo = open(SERV_FIFO, O_WRONLY, 0);
    write(writefifo, buff, len);

    //打开自己的FIFO 读取来自服务器的应答数据
    readfifo = open(fifoname, O_RDONLY, 0);
    while ((n = read(readfifo, buff, MAXLINE)) > 0)
        write(STDOUT_FILENO, buff, n);
    close(readfifo);
    unlink(fifoname);

    exit(0);
}

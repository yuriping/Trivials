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
    int     readfifo;        //从自己的FIFO读
    int     writefifo;       //向进程对应的FIFO写
    int     fd;              //文件描述符
    char    buff[MAXLINE];
    char    fifoname[MAXLINE];
    char*   ptr;
    ssize_t n;

    //创建服务器的FIFO
    if ((mkfifo(SERV_FIFO, FILE_MODE) < 0) && (errno != EEXIST))
        printf("can't create %s", SERV_FIFO);

    //打开服务器FIFO的读、写端
    readfifo = open(SERV_FIFO, O_RDONLY, 0);

    //读取来自客户端的请求
    while (1)
    {
        if ((n = read(readfifo, buff, MAXLINE)) > 0)
        {
            if (buff[n-1] == '\n')
            {
                buff[--n] = '\0';
            }
            //获取客户端的进程ID号
            if ((ptr = strchr(buff, ' ')) == NULL)
            {
                printf("bogus request: %s", buff); //伪造的客户请求
                continue;
            }
            *ptr ++ = 0;

            //根据客户端进程ID打开客户端FIFO
            snprintf(fifoname, sizeof(fifoname), "./fifo.%s", buff);
            if ((writefifo = open(fifoname, O_WRONLY, 0)) < 0)
            {
                printf("cannot open : %s", fifoname);
                continue;
            }

            //向客户端FIFO中写入客户所请求的文件内容
            if ((fd = open(ptr, O_RDONLY)) < 0)
            { //打开文件出错，向客户端返回错误信息
                 snprintf(buff+n, sizeof(buff) - n, ":can't open, %s\n", strerror(errno));
                 n = strlen(ptr);
                 write(writefifo, ptr, n);
                 close(writefifo);
            }
            while ((n = read(fd, buff, MAXLINE)) > 0)
            {
                write(writefifo, buff, n);
            }

            close(writefifo);
            close(fd);
         }

        sleep(1);
    } //while

    exit(0);
}

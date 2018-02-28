#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>


#define FIFO_REQE   "/tmp/DL645reqe"
#define FIFO_ACK    "/tmp/DL645ack"
#define N           (1024)


typedef struct
{
    int a;
    char ch;
    char name[10];

    int type;   // 0-b, 1-c
    union
    {
        int b;
        char c;
    } u;
} S;

typedef struct
{
    int a, b;   // 两个加数
    int res;    // 结果
} A;


int main(void)
{
    int fdw, fdr;
    if ((fdw = open(FIFO_REQE, O_WRONLY | O_NONBLOCK, 0)) == -1)
    {
        printf("Open fifo request to write failed...\n");exit(1);
    }

    fd_set wset, rset;

    S s;
    s.a = 17;
    s.ch = 'u';
    strcpy(s.name, "zhuxinping");
    s.type = 0;
    s.u.b = 10;

    int idx;
    for (idx = 0; idx < 10; idx ++)
    {
        s.u.b = idx;
        char * buf = (char *)(&s);
        int len = sizeof(s);

        FD_ZERO(&wset);     FD_SET(fdw, &wset);
        if (select(fdw + 1, NULL, &wset, NULL, NULL))
        {
            // 准备接收服务器返回结果
            if ((fdr = open(FIFO_ACK, O_RDONLY | O_NONBLOCK, 0)) == -1)
            {
                printf("Open fifo request to read failed...\n");exit(1);
            }

            if (write(fdw, buf, len) != len)
                printf("write fifo failed...\n");

            // 读取服务器返回的结果
            FD_ZERO(&rset);     FD_SET(fdr, &rset);
            if (select(fdr + 1, &rset, NULL, NULL, NULL))
            {
                A a;
                char * ack = (char *)(&a);
                int nread = read(fdr, ack, sizeof(A));
                if (nread == sizeof(A))
                    printf("++++++++ { %d + %d = %d}\n", a.a, a.b, a.res);
                else
                {
                    printf("server guanbile ...\n");
                }
            }

            close(fdr);
        }
    }

    close(fdw);

    exit(0);
}

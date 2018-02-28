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
    int     fdr, fdw;
    fd_set  rset, wset;
    int     nsel;
    int     nread;
    char    buf[N];

    if ((mkfifo(FIFO_REQE, O_CREAT | O_EXCL) < 0) && (errno != EEXIST))
    {
        printf("fifo request can not be created...\n");exit(1);
    }
    if ((fdr = open(FIFO_REQE, O_RDONLY | O_NONBLOCK, 0)) == -1)
    {
        printf("open fifo request to read failed...\n");exit(2);
    }

    if ((mkfifo(FIFO_ACK, O_CREAT | O_EXCL) < 0) && (errno != EEXIST))
    {
        printf("fifo ack can not be created...\n");exit(3);
    }

//    sleep(5);

    while (1)
    {printf("shoudaole......\n");
        FD_ZERO(&rset);     FD_SET(fdr, &rset);
        nsel = select(fdr + 1, &rset, NULL, NULL, NULL);
        if (nsel < 0)       // select 本身出错了
        {
            if (errno != EINTR) // 不是被信号中断的
                printf("select fault...\n");   // 考虑是退出循环还是继续
            else                // select 被信号中断
                printf("select interrupted by signal...\n");
            continue;
        }
        else if (nsel == 0) // select 超时了
        {
            // 我们就不可能运行到这里
        }
        else    // nsel > 0     这个 fifo 有东西能读
        {
            if (FD_ISSET(fdr, &rset))   // 关注的 fifo 可读
            {
                nread = read(fdr, buf, sizeof(S)); //nread = read(fdr, buf, sizeof(S)); // nread = read(fdr, buf, N);
                if (nread == 0)
                {
                    printf("versus write closed...exit\n"); break;
                }
                else if (nread < 0)
                {
                    printf("fifo read failed...\n");continue; // 还要判断是不是要再读一次
                }
                else    // nread > 0    // 真有东西读到了
                {
                    int cnt = nread / sizeof(S);
                    int idx;
                    S * ps;
                    for (idx = 0, ps = (S *)buf; idx < cnt; idx ++, ps ++ )
                    {
                        printf("a = %d, ch = %c, name = %s, [  ", ps->a, ps->ch, ps->name);
                        if (ps->type == 0)
                            printf("%d ]\n", ps->u.b);
                        else if (ps->type == 1)
                            printf("%c ]\n", ps->u.c);
                        else
                            printf("bad type...\n");

                        A a;
                        a.a = ps->a;
                        a.b = ps->u.b;
                        a.res = a.a + a.b;

                        // 准备返回结果，必须等到客户打开接收fifo读才能写
                        if ((fdw = open(FIFO_ACK, O_WRONLY, 0)) == -1)
                        {
                            printf("Open fifo ack to write failed...\n");continue;
                        }

                        FD_ZERO(&wset);     FD_SET(fdw, &wset);
                        if (select(fdw + 1, NULL, &wset, NULL, NULL))
                        {
                            write(fdw, (char *)(&a), sizeof(A));
                        }

                        close(fdw);
                    }
                }
            }
            else    // 关注的 fifo 不可读
            {
                printf("not my fifo to select...\n");continue;
            }
        }
    }

    exit(0);
}

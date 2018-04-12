/*
 * 演示功能：
 * fifo 的对等通信，不是C/S
 * peer_0
 */


#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>


#define FIFO_RD     "/tmp/1_0"      // peer 1 发给 peer 0 的 fifo
#define FIFO_WR     "/tmp/0_1"      // peer 0 发给 peer 1 的 fifo


#define N           (1024)


//////// 信号处理 ////////////////////////////////

typedef void SIG_HND_T(int);

SIG_HND_T * SigHndInit(int signum, SIG_HND_T * hnd)
{
    struct sigaction act, oact;

    act.sa_handler = hnd;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    if (SIGALRM == signum)
    {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    }
    else
    {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }

    if (sigaction(signum, &act, &oact) < 0)
        return SIG_ERR;

    return oact.sa_handler;
}

void SigHnd_SIGPIPE_WrFIFOWithNoRd(int signum)
{
    signum = signum;

    printf("write fifo : versus reading closed...\n");

    return;
}

//////// 收发过程 ////////////////////////////////

typedef struct
{
    int i;
} Pac;

pthread_t   thr_recv, thr_snd;
int         fdRdR, fdRdAid, fdWrAid, fdWrW;
fd_set      rset;
int         nsel;
int         nread;
char        buf[N];

void * Recv(void * prm)
{
    for (;;)
    {
        // 等待 fifo 收到数据
        FD_ZERO(&rset);     FD_SET(fdRdR, &rset);
        nsel = select(fdRdR + 1, &rset, NULL, NULL, NULL);
        if (nsel < 0)       // select 本身出错了
        {
            if (errno != EINTR) // 不是被信号中断的
                printf("select fault...\n");   // 考虑是退出循环还是继续
            else                // select 被信号中断
                printf("select interrupted by signal...\n");
            continue;   // 不用退出，再等待下次数据行了
        }
        else if (nsel == 0) // select 超时了
        {
            // 我们就不可能运行到这里
        }
        else    // nsel > 0     fifo 有东西能读
        {
            if (FD_ISSET(fdRdR, &rset))   // 关注的 fifo 可读
            {
                nread = read(fdRdR, buf, sizeof(Pac));  // 一次只读一个结构的长度，如果还有，留到下次处理
                if (nread == 0) // 客户关闭了请求 fifo 的写端，要重以读打开这个 fifo
                {
                    printf("versus write closed...exit\n");
                    close(fdRdR);
                    if ((fdRdR = open(FIFO_RD, O_RDONLY | O_NONBLOCK, 0)) == -1)    // 如果再次打不开了，就算了
                    {
                        printf("open fifo request to read failed...\n");break;
                    }
                    close(fdRdAid);
                    fdRdAid = open(FIFO_RD, O_WRONLY | O_NONBLOCK, 0);
                }
                else if (nread < 0)     // 读错误
                {
                    printf("fifo read failed...\n");continue; // 还要判断是不是要再读一次
                }
                else    // nread > 0    // 真有东西读到了
                {
                    // 接收请求，并计算结果
                    Pac * p = (Pac *)buf;
                    printf("p.i = %c\n", p->i);
                }
            }
            else    // 关注的 fifo 不可读
            {
                printf("not my fifo to select...\n");continue;
            }
        }
    }

    close(fdRdR);
    close(fdRdAid);

    pthread_exit((void *)0);
}

void * Snd(void * prm)
{
    for (;;)
    {
        int idx;
        for (idx = 0; idx < 10; idx ++)
        {
            Pac p;
            p.i = 10 + idx;
            if (write(fdWrW, (char *)(&p), sizeof(Pac)) != sizeof(Pac))
            {
                // 如果写失败了，说明对端没有人读，应用应该处理这种情况，是重传还是保存？
                // 反正不能干预对端，更不能自己打开一个读描述符，只能等着对端有人读了……
                printf("write fifo failed...\n"); // continue;
            }

            sleep(1);
        }
    }

    close(fdWrW);

    pthread_exit((void *)0);
}

//////// 主函数 ////////////////////////////////

int main(int argc, char * argv[])
{
    /*
     * 初始化 FIFO
     */
    /* 修改权限屏蔽字，以使对 FIFO 权限的指定能成功 */
    mode_t oldmask = umask(0000);
    printf("old mask = %d\n", oldmask);

    /* 处理我读的 FIFO */
    // 创建，所有人拥有读写权限，因为不一定谁会以什么身份使用这个fifo
    if ((mkfifo(FIFO_RD, O_CREAT | O_EXCL | (S_IRWXU | S_IRWXG | S_IRWXO)) < 0) && (errno != EEXIST))
    {
        printf("Read fifo created failed...\n"); exit(1);
    }
    // 读打开
    if ((fdRdR = open(FIFO_RD, O_RDONLY | O_NONBLOCK, 0)) == -1)
    {
        printf("Read fifo open reading failed...\n"); exit(2);
    }
    // 以写方式打开，只为防止没有写对端时退出，不会写，如打不开也往下走
    if ((fdRdAid = open(FIFO_RD, O_WRONLY | O_NONBLOCK, 0)) == -1)
        printf("Read fifo open writing failed...\n");

    /* 处理我写的 FIFO */
    // 创建，所有人拥有读写权限，因为不一定谁会以什么身份使用这个fifo
    if ((mkfifo(FIFO_WR, O_CREAT | O_EXCL | (S_IRWXU | S_IRWXG | S_IRWXO)) < 0) && (errno != EEXIST))
    {
        printf("Write fifo created failed...\n"); exit(3);
    }
    // 恢复之前默认的权限屏蔽字
    mode_t newmask = umask(oldmask);
    // 以读方式打开，只为等一下以写打开，之后便关闭这个描述符，如打不开出错
    if ((fdWrAid = open(FIFO_WR, O_RDONLY | O_NONBLOCK, 0)) == -1)
    {
        printf("Write fifo open reading failed...\n"); exit(4);
    }
    // 写打开
    if ((fdWrW = open(FIFO_WR, O_WRONLY | O_NONBLOCK, 0)) == -1)
    {
        printf("Write fifo open writing failed...\n"); exit(5);
    }
    // 关闭读打开的描述符，如失败，出错，否则等一下真正写的时候不能区分对端有没有人读
    if (close(fdWrAid) == -1)
    {
        printf("Write fifo close reading failed...\n"); exit(6);
    }

    /* 信号处理 */
    // SIGPIPE
    if (SIG_ERR == SigHndInit(SIGPIPE, SigHnd_SIGPIPE_WrFIFOWithNoRd))
    {
        fprintf(stderr, "Signal Initiation Failed\n"); exit(7);
    }

    pthread_create(&thr_recv, NULL, Recv, NULL);

    pthread_create(&thr_snd, NULL, Snd, NULL);

    for (;;)
    {
        printf("peer 0 : main...\n");
        sleep(5);
    }

    exit(0);
}

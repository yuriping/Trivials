/*
 * 演示功能：
 * 利用双管道双向传递数据的客户端 Demo，在 fifo_cli_full.c 基础上将 write 操作去掉 select 动作；
 * 并且结果 FIFO 始终打开，而不像在 full 版本中，一次应答打开、关闭一次；
 * 并增加了对于对端读关闭的响应；
 *
 * 工作原理描述：
 */

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>


#define FIFO_REQE   "/tmp/DL645reqe"
#define FIFO_ACK    "/tmp/DL645ack"


typedef void SIG_HANDLER_T(int);

SIG_HANDLER_T * SigHandlerInit(int signum, SIG_HANDLER_T * handler)
{
    struct sigaction act, oact;

    act.sa_handler = handler;
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

static bool wrEn = true;   // 表示对端的读没有关闭，这个 FIFO 可以写

void SigHandler_SIGPIPE_WriteToFIFOWithNoReading(int signum)
{
    (void)signum;

    printf("Versus reading closed...\n");

    wrEn = false;   // 这个 FIFO 不能写了

    return;
}

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
    int     fdReqeW, fdAckR;
    fd_set  rset;

    // 打开提交请求的 fifo
    if ((fdReqeW = open(FIFO_REQE, O_WRONLY | O_NONBLOCK, 0)) == -1)
    {
        printf("Open fifo request to write failed...\n");exit(1);
    }

    // 打开接收结果的 fifo
    if ((fdAckR = open(FIFO_ACK, O_RDONLY | O_NONBLOCK, 0)) == -1)
    {
        printf("Open fifo ack to read failed...\n");exit(2);
    }

    // 捕捉 SIGPIPE 信号
    if (SIG_ERR == SigHandlerInit(SIGPIPE, SigHandler_SIGPIPE_WriteToFIFOWithNoReading))
    {
        fprintf(stderr, "Signal Initiation Failed\n");
        exit(3);
    }

    int idx;
    for (idx = 0; idx < 10; idx ++)
    {sleep(1);
        // 形成请求
        S s;
        s.a     = 17;
        s.ch    = 'u';
        strcpy(s.name, "zhuxinping");
        s.type  = 0;
        s.u.b   = idx;

        // 提交请求，如果提交失败，则进入下次请求，提交请求 fifo 肯定可写，不用 select
        if (!wrEn)   // 先判断这个 FIFO 还能不能写了
            break;

        if (write(fdReqeW, (char *)(&s), sizeof(s)) != sizeof(s))
        {
            printf("write fifo failed...\n");continue;
        }


        // 读取服务器返回的结果，必须用 select，否则 fifo 一直都是可读的，会 read 到长度为 0 的内容
        FD_ZERO(&rset);     FD_SET(fdAckR, &rset);
        if (select(fdAckR + 1, &rset, NULL, NULL, NULL))   // 肯定是我们关心的 ack fifo 读就绪，因为 rset 中就那么一个文件
        {
            A a;
            int nread = read(fdAckR, (char *)(&a), sizeof(A));
            if (nread == sizeof(A)) // 读到了结果数据
            {
                printf("++++++++ { %d + %d = %d}\n", a.a, a.b, a.res);
            }
            else if (nread == 0)    // 如果==0，说明对端关闭了结果 fifo，要再次打开
            {
                close(fdAckR);
                if ((fdAckR = open(FIFO_ACK, O_RDONLY | O_NONBLOCK, 0)) == -1) // 如果再次打开失败，则终止程序
                    break;
            }
            else                    // 不是结果数据，就当这次没收到
                continue;
        }
    }

    close(fdReqeW);

    close(fdAckR);

    exit(0);
}

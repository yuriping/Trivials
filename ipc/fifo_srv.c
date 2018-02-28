/*
 * 演示功能：
 * 利用双管道双向传递数据的服务器 Demo，在 fifo_srv_full.c 基础上将 write 操作去掉 select 动作；
 *
 * 工作原理描述：
 */

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
    int     fdReqeR, fdReqeAid, fdAckW = -1;
    fd_set  rset, wset;
    int     nsel;
    int     nread;
    char    buf[N];

    // 创建请求 fifo
    if ((mkfifo(FIFO_REQE, O_CREAT | O_EXCL) < 0) && (errno != EEXIST))
    {
        printf("fifo request can not be created...\n");exit(1);
    }
    // 打开请求 fifo
    if ((fdReqeR = open(FIFO_REQE, O_RDONLY | O_NONBLOCK, 0)) == -1)
    {
        printf("open fifo request to read failed...\n");exit(2);
    }
    // 以写方式打开请求 fifo，只是为了防止没有写对端的时候我们自己退出，我们不会向请求 fifo 中写，如果打不开也往下走
    if ((fdReqeAid = open(FIFO_REQE, O_WRONLY | O_NONBLOCK, 0)) == -1)
        printf("Open fifo ack to write failed...\n");

    // 创建结果 fifo
    if ((mkfifo(FIFO_ACK, O_CREAT | O_EXCL) < 0) && (errno != EEXIST))
    {
        printf("fifo ack can not be created...\n");exit(3);
    }

//    sleep(5);

    while (1)
    {printf("shoudaole......\n");//sleep(1);
        // 等待请求 fifo 收到数据
        FD_ZERO(&rset);     FD_SET(fdReqeR, &rset);
        nsel = select(fdReqeR + 1, &rset, NULL, NULL, NULL);
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
            if (FD_ISSET(fdReqeR, &rset))   // 关注的 fifo 可读
            {
                nread = read(fdReqeR, buf, sizeof(S));  // 一次只读一个结构的长度，如果还有，留到下次处理
                if (nread == 0) // 客户关闭了请求 fifo 的写端，要重以读打开这个 fifo
                {
                    printf("versus write closed...exit\n");
                    close(fdReqeR);
                    if ((fdReqeR = open(FIFO_REQE, O_RDONLY | O_NONBLOCK, 0)) == -1)    // 如果再次打不开了，就算了
                    {
                        printf("open fifo request to read failed...\n");break;
                    }
                    close(fdReqeAid);
                    fdReqeAid = open(FIFO_REQE, O_WRONLY | O_NONBLOCK, 0);
                }
                else if (nread < 0)     // 读错误
                {
                    printf("fifo read failed...\n");continue; // 还要判断是不是要再读一次
                }
                else    // nread > 0    // 真有东西读到了
                {
                    // 接收请求，并计算结果
                    S * ps = (S *)buf;
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

                    // 准备返回结果，写方式打开结果 fifo，必须等客户已经以读方式打开，且只能打开一次
                    if (fdAckW == -1)
                        if ((fdAckW = open(FIFO_ACK, O_WRONLY, 0)) == -1)
                        {
                            printf("Open fifo ack to write failed...\n");continue;
                        }

                    //返回结果，如果写失败，不要在意，去等下一次请求吧，到这里了，结果 fifo 肯定能写，不用 select，麻烦
                    if (write(fdAckW, (char *)(&a), sizeof(A)) != sizeof(A))
                    {
                        printf("write ack failed...\n");continue;
                    }
                }
            }
            else    // 关注的 fifo 不可读
            {
                printf("not my fifo to select...\n");continue;
            }
        }
    }

    // 到这里了，说明请求 fifo 的读描述符、辅助描述符都已经被关闭了，不用管它们了
    close(fdAckW);
    // fdAckW = -1;     // 这一句不写也行，程序都结束了，还管什么无效值

    exit(0);
}

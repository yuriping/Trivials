/*
 * 使用 select 的 tcp 迭代服务器
 * 较原书程序：
 *   这其中有accept使用较完整的用法，处理被信号中断及客户端夭折的情况；
 *   并在关闭一个套接字后，增加了更新 maxi（客户端套接字占用的最大数组下标）的操作，防止之后遍历的低效；
 */

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
    int                 listenfd, connfd;   // 监听套接字、connect返回的新客户通信套接字
    struct sockaddr_in  srvaddr, cliaddr;
    socklen_t           addrlen;            // 客户端地址长度
    int                 maxfd;              // select使用的最大描述符参数
    fd_set              allset, rset;       // 所有要读的套接字描述符集合、当前select的工作套接字描述符集合
    int                 nready;             // 本次select返回多少就绪的描述符
    int                 client[FD_SETSIZE]; // 存放已连接的客户端套接字描述符的数组
    int                 i, maxi, clifd;     // 处理客户集合，数组下标、最大已用下标、当前取出客户套接字；
    ssize_t             n;                  // 读操作的结果值
    char                buf[1024];

    /* 创建监听套接字 */
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "socket creatation failed...\n");
        exit(1);
    }
    // 令套接字在关闭后可重用
    int reuseAddrListensockFlag = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuseAddrListensockFlag, sizeof(reuseAddrListensockFlag));

    /* 设置服务器地址 */
    bzero(&srvaddr, sizeof(srvaddr));
    srvaddr.sin_family      = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);    // 本地所有 IP，枚举为 0
    srvaddr.sin_port        = htons(9877);

    /* 服务器地址绑定监听套接字 */
    if (bind(listenfd, (const struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0) // 强制类型转换，sockaddr_in 到 sockaddr
    {
        fprintf(stderr, "Binding failed...\n");
        exit(2);
    }

    /* 开始监听 */
    if (listen(listenfd, 1024) < 0)
    {
        fprintf(stderr, "Listening failed...\n");
        exit(3);
    }

    // 从这里开始做与套接字无关的初始化

    /* 初始化客户套接字数组 */
    // 数组初始化为无效套接字（-1），表示所有元素都可用
    for (i = 0; i < FD_SETSIZE; i ++)
        client[i] = -1;
    // 当前最大已用下标
    maxi = -1;  // 没有任何元素被使用

    /* select 参数初始化 */
    // 当前最大描述符就是监听套接字
    maxfd = listenfd;
    // 初始形成所有读操作的描述符集合，仅包含监听套接字
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    // 从这里开始正式工作

    while (1)
    {
        /* 开始select */
        rset = allset;  // 重新设置读集合，因为可能在上次select中被修改
        if ((nready = select(maxfd + 1, &rset, NULL, NULL, NULL)) < 0)  // select 发生了错误
        {
            if (errno != EINTR) // 不是被信号中断的
                printf("select fault...\n");    // 考虑是退出循环还是继续
            else                // 被信号中断
                printf("select interrupted by signal...\n");
            continue;
        }
        else if (nready == 0)   // select 超时了
        {
            printf("select Time out\n");    continue;   // 我们就不可能运行到这里
        }

        /* 处理监听套接字的读，有新客户连接请求到来 */
        if (FD_ISSET(listenfd, &rset))  // 确实有新的客户端连接
        {
            /*
             * BUG :原书程序存在 bug，可能发生已有客户连接建立，进入通信过程，
             * 当前又一次 accept，但客户端夭折，如果使用原书封装的 Accept，将在其中等待下次有新客户端到来，而不再响应已建立客户端的读写；
             */
            /*
             * 走到这里了，肯定有新的客户端到来，但仍然可能出现 accept 被信号中断，或客户端夭折的情况
             * 如果被信号中断，再 accept 一次；
             * 如果客户端夭折，就不要再 accept 了，这个连接就无效了；
             */
            /*
             * 程序：accept 就是要循环执行，以应对信号中断，一直到成功或客户端夭折才退出
             */
            bool acptSucc = false;  // 初始认为 accept 操作未成功
            do
            {
                addrlen = sizeof(cliaddr);
                connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &addrlen);
                // 如果成功读到了客户端信息
                if (connfd >= 0)    // NOTE : 只要非负就是正确的套接字描述符，不要以为 0 不是有效的描述符
                {
                    acptSucc = true;
                    break;
                }
                else if (connfd < 0)    // 如果读取失败
                {
                    // 如果被信号中断，再读一次
                    if (errno == EINTR)
                    {
                        printf("accept interrupted by signal...\n"); continue;
                    }
                    // 如果客户端夭折，不再accept了
#ifdef  EPROTO  // 判断客户端夭折，对不同系统有不同的判断方法
                        // NOTE : 这个条件可以和后面的else分支合并，这里只是为了表示怎么判断客户端夭折
                    else if (errno == EPROTO || errno == ECONNABORTED)
#else
                    else if (errno == ECONNABORTED)
#endif
                    {
                        printf("accept : client aborted...\n"); break;
                    }
                    else
                    {
                        printf("accept : other fault...\n"); break;
                    }
                }
            } while ((connfd < 0) && (errno == EINTR)); // 只有在没 accept 成功，且失败原因是被信号中断时，才反复执行 accept

            // 成功接受了客户端的连接
            if (acptSucc)
            {
                char ip[16];
                printf("new client : %s : %d, fd = %d\n",
                    inet_ntop(AF_INET, &cliaddr.sin_addr, ip, 16), ntohs(cliaddr.sin_port), connfd);

                // 存放新客户端的通信套接字，在数组中找地方
                for (i = 0; i < FD_SETSIZE; i ++)
                    if (client[i] < 0)
                    {
                        client[i] = connfd; break;
                    }

                // 客户端不能太多，多过数组长度；如果本次成功接收客户端，并存了通信套接字，则更新相关量，
                // 如果不接收该客户端，也不退出，继续处理已连接客户端的读写
                if (i < FD_SETSIZE)
                {
                    FD_SET(connfd, &allset);                    // 增加读描述符集合
                    maxfd = connfd > maxfd ? connfd : maxfd;    // 更新最大文件描述符
                    maxi  = i      > maxi  ? i      : maxi;     // 更新数组最大使用下标
                }

                // 如果本次select只有一个新建客户端，则不用继续检查已有客户端的读了
                if (-- nready <= 0)
                    continue;
            }
        }

        /* 处理已建立客户端的读写，看看哪些客户端这次发来了数据 */
        for (i = 0; i <= maxi; i ++)
        {
            // 如果数组当前下标无效，检查下一个
            if ((clifd = client[i]) < 0)
                continue;

            // 如果当前客户端有数据到来
            if (FD_ISSET(clifd, &rset))
            {
                if ((n = read(clifd, buf, 1024)) == 0)  // 该客户端关闭了 TODO : 读也可能被信号中断，得处理
                {
                    printf("client disconnect : fd = %d\n", clifd);// 不管具体的 IP 信息，就是个 demo 没必要那么费劲
                    close(clifd);               // 关闭这个客户端的套接字，一定要自己执行一次关闭，完成 TCP 连接关闭的后半段
                    FD_CLR(clifd, &allset);     // 从读集合中清除这个客户端通信套接字
                    client[i] = -1;             // 收回数组空间

                    /*
                     * BUG  : 原书没有这一段，这里应该对 maxi 进行更新，
                     *        否则会影响效率，下次继续遍历已不存在的客户端，且这个值只能增长
                     */
                    /*
                     * 如果结束的连接文件描述符在 maxi 下标，则从其之前的一个下标开始降序遍历，
                     * 直到第一个不是 -1 的下标为新的 maxi；注意全都没有了的情况，maxi 为 -1；
                     */
                    if (i == maxi)   // 当前关闭的客户端信息存在数组最后一个有效元素时才需要重新找 maxi
                    {
                        int j;
                        for (j = i - 1; j >= 0; j --)
                            if (client[j] >= 0)
                                break;
                        maxi = j;
                    }
                    printf("maxi = %d\n", maxi);
                }
                else                                    // 该客户端有数据到达
                {
                    write(clifd, buf, n);   // TODO : 写操作也可能被中断或出错，得处理
                }

                // 如果本次 select 的读源都处理完了，就不用再继续遍历了
                if (-- nready <= 0)
                    break;
            }
        }
    }

    exit(0);
}

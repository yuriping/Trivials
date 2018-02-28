/*
 * 使用 select 的 tcp 客户端
 * TODO : connect、read、write失败，要不要重复啊？
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
    int    sockfd;
    struct sockaddr_in servaddr;
    int    ret;

    /* 检查参数 */
    if (argc != 2)
    {
        fprintf(stderr, "usage: tcpclisel <IPaddress>\n");
        exit(1);
    }

    /* 创建套接字 */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "socket creatation failed...\n");
        exit(2);
    }

    /* 绑定服务器地址 */
    bzero(&servaddr, sizeof(servaddr));
    if ((ret = inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) == 0)
    {
        fprintf(stderr, "presentation to numeric failed: format invalid...\n");
        exit(3);
    }
    else if (ret == -1)
    {
        fprintf(stderr, "presentation to numeric failed: error...\n");
        exit(4);
    }
    // 正确转换时返回值为1，这里不再检查，运行到这里了，不是-1，不是0，就认为正确了
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(9877);

    /* 连接服务器 */
    // 实际参数是 sockaddr_in，要强制转换成 sockaddr，这两个是一样的
    // TODO : 如果这个是被打断的怎么办？要不要重新执行一下？
    if (connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        fprintf(stderr, "connection failed...\n");
        exit(5);
    }

    /* 从这里开始可以向服务器发、收数据了 */
    int maxfd;
    int stdinfd = fileno(stdin);
    bool stdineof = false;
    fd_set rset;
    char buf[1024];
    int n;

    FD_ZERO(&rset);
    while (1)
    {
        // 如果标准输入没结束，则select时要等待来自stdin的数据
        if (stdineof == false)
        {
            FD_SET(stdinfd, &rset);
            maxfd = stdinfd + 1;
        }
        // 总是等待来自socket的数据
        FD_SET(sockfd, &rset);
        if (sockfd > stdinfd)
            maxfd = sockfd + 1;

        // 选取两个文件描述符中大的加一做select参数
//        maxfd = (stdinfd > sockfd ? stdinfd : sockfd) + 1;

        // 执行核心
        if ((n = select(maxfd, &rset, NULL, NULL, NULL)) < 0)
        {
            printf("select failed...\n");
            continue;
        }

        // 处理套接口数据
        if (FD_ISSET(sockfd, &rset))
        {
            if ((n = read(sockfd, buf, 1024)) == -1)    // 读错误，TODO : 这个可能要改，重复读还是怎么办……
            {
                printf("reading failed...\n");
                continue;
            }
            else if (n == 0)    // 如果服务器关闭了
            {
                // 如果此时还有标准输入还有数据，说明服务器提前关闭了，不正常
                if (stdineof == false)
                    printf("Server closed too early...\n");
                // 无论如何都要退出程序，服务器都没了，还干啥？
                break;
            }
            else    // 正常收到一个数据
            {
                if (write(fileno(stdout), buf, n) != n) // 写到标准输出错误，TODO : 可能要改，重复写还是怎么办……
                {
                    printf("write to stdout failed...\n");
                    continue;
                }
            }
        }

        // 处理标准输入的数据
        if (FD_ISSET(stdinfd, &rset))
        {
            // 如果标准输入结束，则置标志，并关闭写半连接
            if ((n = read(stdinfd, buf, 1024)) == 0)
            {
                stdineof = true;
                shutdown(sockfd, SHUT_WR);
                FD_CLR(stdinfd, &rset);
                continue;
            }

            // 标准输入有数据，正常发送到服务器
            if (write(sockfd, buf, n) != n)
            {
                printf("write to socket failed...\n");
                continue;
            }
        }
    }

    exit(0);
}

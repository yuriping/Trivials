/*
 * udp 通信的服务端
 * TODO : 如果执行中，服务器退出，客户端再执行一次消息发送，以后即使服务器重启，也连不上了；
 *        但如果服务器退出到重启之间，客户端不执行消息发送，就可以正常继续工作；
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


/* 收发缓存的最大长度 */
#define BUF_LEN_MAX     1024


int main(int argc, char * argv[])
{
    int                 sockfd;             // 服务器收发的套接字
    struct sockaddr_in  srvaddr, cliaddr;   // 服务器、客户端的地址变量
    socklen_t           cliaddlen;          // 接收时，客户地址结构的实际长度
    int                 n;                  // 收信息的实际长度
    char                buf[BUF_LEN_MAX];   // 收发消息的缓存

    /* 创建读写套接字 */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        fprintf(stderr, "%s\n", "socket creating failed...");
        exit(1);
    }

    /* 设置服务器地址 */
    bzero(&srvaddr, sizeof(srvaddr));
    srvaddr.sin_family      = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);    // 本地所有 IP，枚举为 0
    srvaddr.sin_port        = htons(9877);

    /* 绑定服务器地址, bind 成功返回 0，失败返回 -1 NOTE : 记得地址参数强制类型转换 */
    if ((bind(sockfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr))) < 0)
    {
        fprintf(stderr, "%s\n", "socket binding failed...");
        exit(2);
    }

    /* 循环接收客户端消息，并返回 */
    for (;;)
    {
        // NOTE : 不用像 TCP 那样 有 accept，直接接收
        cliaddlen = sizeof(cliaddr);

        // NOTE : 记得地址参数强制类型转换，并传客户地址结构变量的地址，准备接收实际地址长度，可能和定义的不一样
        n = recvfrom(sockfd, buf, BUF_LEN_MAX, 0, (struct sockaddr *)&cliaddr, &cliaddlen);
        if (n < 0)  // 如果接收错误，丢掉，接收下一条
        {
            fprintf(stderr, "%s\n", "recvfrom failed...");
            continue;
        }

        // NOTE : 记得地址参数强制类型转换
        sendto(sockfd, buf, n, 0, (struct sockaddr *)&cliaddr, cliaddlen);
    }

    exit(0);
}

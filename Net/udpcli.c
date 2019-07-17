/*
 * udp 通信的客户端
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
    struct sockaddr_in  cliaddr;            // 客户端的地址变量
    socklen_t           cliaddlen;          // 接收时，客户地址结构的实际长度


    int                 sockfd;                 // 要发送到的服务器的套接字
    struct sockaddr_in  srvaddr;                // 要发送到的服务器的地址
    int                 ret;
    char                sndbuf[BUF_LEN_MAX];    // 发消息的缓存
    char                recvbuf[BUF_LEN_MAX];   // 收消息的缓存
    int                 n;                      // 收发信息的实际长度

    /* 参数检查并指定服务器地址 */
    // 必须指定要发送的 UDP 服务器的地址
    if (argc != 2)
    {
        fprintf(stderr, "%s\n", "usage: udpcli <IPaddress>");   exit(1);
    }
    // 看看地址是否合法并绑定服务器地址
    bzero(&srvaddr, sizeof(srvaddr));
    if ((ret = inet_pton(AF_INET, argv[1], &srvaddr.sin_addr)) == 0)
    {
        fprintf(stderr, "presentation to numeric failed: format invalid...\n"); exit(2);
    }
    else if (ret == -1)
    {
        fprintf(stderr, "presentation to numeric failed: error...\n");  exit(3);
    }
    // 正确转换时返回值为1，这里不再检查，运行到这里了，不是-1，不是0，就认为正确了
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port   = htons(9877);

    /* 创建读写套接字 */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        fprintf(stderr, "%s\n", "socket creating failed...");   exit(4);
    }

    /* 发送并接收的过程 */
    for (;;)
    {
        /* 从标准输入中得到一个字符串 */
        if ((fgets(sndbuf, BUF_LEN_MAX, stdin)) == NULL)
        // NOTE : 到这里缓存中就是0结尾的字符串，哪怕开始长，第二次短，也正常，用strlen就可得到其长度，fgets 能自动填充字符串结束符
        /*
         * fgets 到文件尾，对 stdin 来说，就是没有输入了，则返回 NULL ，
         */
        {
            if (ferror(stdin))  // 如果标准输入错误，就直接退出
            {
                fprintf(stderr, "%s\n", "fgets failed..."); exit(5);
            }
            else                // 如果标准输入不错误，那就是输入结束了，退出主流程，就能看到那一句 input over...
                break;
        }

        /* 向服务器发送，必须长度相等，才算成功 */
        n = sendto(sockfd, sndbuf, strlen(sndbuf), 0, (struct sockaddr *)&srvaddr, sizeof(srvaddr));
        if (n != strlen(sndbuf))
        {
            fprintf(stderr, "%s\n", "sendto failed..."); exit(6);
        }

        /* 接收服务器的返回，必须大于等于 0 才算成功 */
        if ((n = recvfrom(sockfd, recvbuf, BUF_LEN_MAX, 0 ,NULL, NULL)) < 0)    // 不关心从哪里来的消息
                                                                                // NOTE : 有风险，可能不是来自服务器的
        {
            fprintf(stderr, "%s\n", "recvfrom failed...");  exit(7);
        }
        recvbuf[n] = 0; // 加上字符终止符，这个和fgets不一样，那个会自己填，这个没有，
                        // 因为收到的数据中可能会有0，这个例子收到的肯定是可显示字符，所以不会有0，为了方便显示，就直接填 0 了

        /* 向标准输出中显示收到的字符串 */
        if ((fputs(recvbuf, stdout)) == EOF)    // 标准输出错误，会返回 EOF
        {
            fprintf(stderr, "%s\n", "fputs failed..."); exit(8);
        }
    }

    printf("input over...\n");

    exit(0);
}

/*========================================================================================================
 传输层基于udp协议的服务端程序
 1：创建套接字
 2：为套接字绑定地址信息
 3：接收数据
 4；发送数据
 5：关闭套接字
========================================================================================================= */
#include<stdio.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>

int main(int argc, char *argv[2])
{
    if(argc != 3)
    {
        printf("Usage: ./main: %s %s\n",argv[1], argv[2]);
        return -1;
    }
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//创建套接字 //ipv4 数据报类型 udp协议
    if(sockfd < 0)
    {
        perror("socket error");
        return -1;
    }
    struct sockaddr_in addr;//定义ipv地址结构
    addr.sin_family = AF_INET;//地址结构类型
    addr.sin_port = htons(atoi(argv[2]));//端口
    addr.sin_addr.s_addr = inet_addr(argv[1]);//将字符串点分十进制ip地址转换成网络字节序

    socklen_t len = sizeof(struct sockaddr_in);//地址长度
    int ret = bind(sockfd, (struct sockaddr*)&addr, len);//地址绑定 操作句柄 地址结构 地质结构大小
    if(ret < 0)
    {
        perror("bind error");
        return -1;
    }

    while(1)
    {
        //接收数据
        char buf[1024] = {0};//缓冲区
        struct sockaddr_in cliaddr;//对端地址结构
        socklen_t len = sizeof(struct sockaddr_in);//对端地址大小
        int ret = recvfrom(sockfd, buf, 1023, 0, (struct sockaddr*)&cliaddr, &len);//操作句柄，缓冲区，缓冲区大小，阻塞，地址，地址大小
        if(ret < 0)
        {
            perror("recvfrom error");
            close(sockfd);
            return -1;
        }

        printf("client say: %s\n", buf);
        //输出数据
        // len = sizeof(struct sockaddr_in);
        memset(buf, 0x00, 1024);//清空缓冲区
        scanf("%s", buf);
        ret = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&cliaddr, len);//操作句柄，缓冲区，缓冲区大小，阻塞，对端地址

        if(ret < 0)
        {
            perror("sendto error");
            close(sockfd);
            return -1;
        }

    }

    close(sockfd);

    return 0;
}


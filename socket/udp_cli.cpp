/*========================================================================================================
 封装实现一个udpsocket类；向外提供更容易使用的udp接口来实现udp通信流程
========================================================================================================= */
#include<iostream>
#include<string>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>

class UdpSocket
{
    int m_sockfd;

public:
    UdpSocket():
        m_sockfd(-1)
    {}

    ~UdpSocket()
    {
        Close();
    }

    bool Socket()//创建套接字
    {
        m_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if(m_sockfd < 0)
        {
            std::cerr << "socket error\n";
        }
        return true;
    }

    bool Bind(const std::string &ip, uint16_t port)//绑定套接字
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        socklen_t len = sizeof(struct sockaddr_in);
        int ret = bind(m_sockfd, (struct sockaddr*)&addr, len);
        if(ret < 0)
        {
            std::cerr << "bind erroe\n";
            return false;
        }
        return true;
    }

    bool Recv(std::string &buf, std::string &ip, uint16_t &port)//接收信息
    {
        char tmp[4096];
        struct sockaddr_in peeraddr;
        socklen_t len = sizeof(peeraddr);
        int ret = recvfrom(m_sockfd, tmp, 4096, 0, (struct sockaddr*)&peeraddr, &len);
        if(ret < 0)
        {
            std::cerr << "recvfrom error\n";
            return false;
        }

        buf.assign(tmp, ret);//给buf开辟指定ret长度的空间,并且拷贝tmp
        port = ntohs(peeraddr.sin_port);// 
        ip = inet_ntoa(peeraddr.sin_addr);/*inet_ntoa,将sockaddr_in.sin_addr正数ip地址转换成字符串类型的ip地址，但是其返回的是缓冲区首地址，
                                            内部使用静态成员变量，故只能初始化一次，为非线程安全的，不能多线程使用*/ 
        return true;
    }

    bool Send(std::string &data, std::string &ip, uint16_t port)//发送
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        socklen_t len = sizeof(struct sockaddr_in);
        int ret = sendto(m_sockfd, &data[0], data.size(), 0, (struct sockaddr*)&addr, len);
        if(ret < 0)
        {
            std::cerr << "sento error\n";
            return -1;
        }

        return true;
    }

    bool Close()//关闭
    {
        if(m_sockfd >= 0)
        {
            close(m_sockfd);
            m_sockfd = -1;
            return true;
        }
        return false;
    }

};

#define CHECK_RET(q) if((q) == false){return -1;}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "./udp_cli serverip serverport\n";
        return -1;
    }
    UdpSocket sock;

    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Bind("192.168.122.132",8000));
    std::string srv_ip = argv[1];
    uint16_t srv_port = atoi(argv[2]);

    while(1)
    {
        std::string buf;
        std::cin >> buf;

        CHECK_RET(sock.Send(buf, srv_ip, srv_port));
        buf.clear();
        CHECK_RET(sock.Recv(buf, srv_ip, srv_port));
        std::cout << "server say" << buf << "\n";
    }

    CHECK_RET(sock.Close());
    return 0;
}



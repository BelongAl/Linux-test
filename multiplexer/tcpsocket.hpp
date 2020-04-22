/******************************************************************************
 * 描述：封装一个tcpsocket类，向外提供接口，能够实现客户端服务端编程流程
 *流程：1：创建套接字
 *      2：绑定地址信息
 *      3：开始监听/发起连接请求
 *      4：获取已完成连接
 *      5：发送数据
 *      6：接收数据
 *      7：关闭套接字     
 ******************************************************************************/

#include<unistd.h>
#include<iostream>
#include<string>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define CHECK_RET(q) if(q == false){return -1;}

class TcpSocket
{
    int m_socket;

public:

    TcpSocket():
        m_socket(-1)
    {}

    ~TcpSocket()
    {
        Close();
    }

	int GetFd()
	{
		return m_socket;
	}

    bool Socket()//创建套接字
    {
        m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(m_socket < 0)
        {
            std::cerr << "socket creat error" << std::endl;
            return false;
        }
        return true;
    }

    bool Bind(std::string &ip, uint16_t port)//套接字绑定
    {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        socklen_t len = sizeof(sockaddr_in);
        int ret = bind(m_socket, (struct sockaddr*)&addr, len);
        if(ret < 0)
        {
            std::cerr << "bind error \n";
            return false;
        }
        return true;
    }

    bool Listen(int backlog = 5)//开始监听//backlog:最大并发连接数
    {
        int ret = listen(m_socket, backlog);
        if(ret < 0)
        {
            std::cerr << "listen error\n";
            return false;
        }
        return true;
    }

    bool Connect(std::string &ip, uint16_t port)//客户端发起连接请求
    {
        sockaddr_in addr;//服务端地址
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(&ip[0]);
        socklen_t len = sizeof(struct sockaddr_in);
        int ret = connect(m_socket, (sockaddr*)&addr, len);
        if(ret < 0)
        {
            std::cerr << "connect erroe\n";
            return false;
        }
        return true;

    }

    void SetFd(int fd)//获取新连接，因为m_socket私有；
    {
        m_socket = fd;
    }

    bool Accept(TcpSocket &newsock)//服务端获取已完成的新连接
    {
        sockaddr_in addr;//并不需要得到客户端的地址，但是也可以获得
        socklen_t len = sizeof(sockaddr_in);
        int fd = accept(m_socket, (sockaddr*)&addr, &len);
        if(fd < 0)
        {
            std::cerr << "accept error\n";
            return false;
        }
        newsock.SetFd(fd);//将新连接的套接字操作句柄给newsock
        return true;
    }

    bool Send(std::string &buf)//发送，若发送到过大，先发送一部分；所以要判断是否发送完
    {
        int ret = send(m_socket, &buf[0], buf.size(), 0);
        if(ret < 0)
        {
            std::cerr << "send error\n";
            return false;
        }
        return true;
    }

    bool Recv(std::string &buf)//接收数据,没有数据会阻塞
    {
        char tmp[4096] = {0};
        int ret = recv(m_socket, tmp, 4096, 0);
        if(ret < 0)
        {
            std::cerr << "recv error\n";
        }
        else if(ret == 0)//若返回值等于0则表示连接已断开
        {
            std::cerr << "peer shutdown\n";
            return false;
        }
        buf.assign(tmp,ret);//拷贝
        return true; 
    }

    bool Close()
    {
        if(m_socket >= 0)//0:标准输入（在标准输入被关闭时，会等于0；（最小位使用））
        {
            close(m_socket);
            m_socket = -1;
            return true;
        }
        return false;
    }

};



/************************
 * 描述：服务端
 * 1：创建套接字
 * 2：绑定地址信息
 * 3：开始监听
 * 4：获取已完成连接
 * 5：接收数据
 * 6：发送数据
 * 7：关闭套接字
*********************** */

#include<stdlib.h>
#include"tcpsocket.hpp"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cout << "plase scanf ./tcp_srv.cpp ip port\n";
        return -1;
    }
    TcpSocket lst_sock;
    lst_sock.Socket();//创建

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    lst_sock.Bind(ip, port);//绑定
    lst_sock.Listen();//监听 

    TcpSocket newsock;
    while(1)
    {
        bool ret = lst_sock.Accept(newsock);//获取新连接,accept是一个阻塞函数(没有新连接则则色)
        if(ret == false)
        {
            continue;
        }
        std::string buf;
        ret = newsock.Recv(buf);//接收数据
        if(ret == false)
        {
            newsock.Close();
            continue;
        }
        std::cout << "client say: " << buf <<  "\n";
        buf.clear();
        std::cin >> buf;
        newsock.Send(buf);//发送数据
    }

    lst_sock.Close();//关闭套接字
    return  0;
}

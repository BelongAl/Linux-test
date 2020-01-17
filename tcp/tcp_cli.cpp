/***************************
 * 描述：这是一个客户端
 * 1：创建套接字
 * 2：绑定地址信息（客户端不需要主动绑定）
 * 3：向服务端发起连接请求
 * 4：发送数据
 * 5：接收数据
 * 6：关闭套接字
************************** */

#include"tcpsocket.hpp"
#include<stdlib.h>

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "plase scanf ./tcp_cli ip port\n";
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);//atoi:将字符串转换成整型
    TcpSocket sock;
    CHECK_RET(sock.Socket());// 创建套接字
    //CHECK_RET(sock.Bind(ip, port));//,绑定地址信息 客户端可以不绑定地址信息
    CHECK_RET(sock.Connect(ip, port));//发起连接请求

    while(1)
    {
        //发送信息
        std::string buf;
        std::cin >> buf;
        bool ret = sock.Send(buf);
        if(ret == false)
        {
            sock.Close();
            return -1;
        }

        //接受信息
        buf.clear();
        ret = sock.Recv(buf);
        if(ret == false)
        {
            sock.Close();
            return -1;
        }
        std::cout << "server say: " << buf << std::endl;
    }
    
    sock.Close();
    return 0;
}

/************************
 * 描述：多进程服务端
 * 1：创建套接字
 * 2：绑定地址信息
 * 3：开始监听
 * 4：获取已完成连接
 * 5：接收数据
 * 6：发送数据
 * 7：关闭套接字
*********************** */

#include<signal.h>
#include<stdlib.h>
#include"tcpsocket.hpp"
#include<unistd.h>
#include<sys/wait.h>

void sigcb(int signo)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);/*SIGCHILD信号是一个非可靠信号，多个进程同时退出
                                            有可能会造成事件丢失，导致有可能会有僵尸进程没有被处理
                                            因此在一次事件回调中，将能够处理的僵尸进程全部处理掉*/
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cout << "plase scanf ./tcp_srv.cpp ip port\n";
        return -1;
    }
    signal(SIGCHLD, sigcb);
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
        //创建子进程进行任务处理，任务分摊：
        //主进程负责连接获取新连接，；子进程负责与客户端进行通信；
        //稳定性高--子进程处理请求出问题不会影响到主进程服务器
        if(fork() == 0)
        {
          /*  if(fork() > 0)
            {可以利用孙子进程进行控制从而退出子进程，孙子进程变成孤儿进程，退出后不会变成僵尸进程
                exit(0);
            }*/ 
            while(1)
            {
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
            newsock.Close();
            exit(0);
        }
        newsock.Close();
    }

    lst_sock.Close();//关闭套接字
    return  0;
}

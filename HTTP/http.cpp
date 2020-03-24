#include<stdlib.h>
#include"tcpsocket.hpp"//自己封装的tcp通信
#include<sstream>

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		std::cout << "plase scanf ./http.cpp ip port\n";
		return false;
	}

	TcpSocket lst_sock;
	lst_sock.Socket();//创建tcp套接字

	std::string ip = argv[1];
	uint16_t port = atoi(argv[2]);

	lst_sock.Bind(ip, port);//绑定
	lst_sock.Listen();//开始监听

	TcpSocket newsock;

	while(1)
	{
		bool ret = lst_sock.Accept(newsock);//获取新连接
		if(ret == false)
		{
			continue;
		}

		std::string buf;//接收缓冲区
		ret = newsock.Recv(buf);//接收数据
		if(ret == false)
		{
			newsock.Close();
			continue;
		}
		
		std::cout << "http req:[" << buf << "]\n";//打印接受的数据

		std::string body = "<html><body><h1>hello world</h1></body></html>";//正文
		std::string blank = "\r\n";//空行
		std::stringstream header;//头部信息
		header << "Content-Length: "<< body.size() << "\r\n";//正文长度,将数据转换成字符串
		header << "Content-Type: text/html\r\n";//显示的数据类型
		
		std::string first = "HTTP/1.1 200 OK\r\n";//响应首行
		newsock.Send(first);//发送首行信息
		std::string tmp(header.str());
		newsock.Send(tmp);//发送头部
		newsock.Send(blank);//发送空行
		newsock.Send(body);//发送正文

		newsock.Close();//关闭，短链接
	}

	lst_sock.Close();//关闭套接字
	return 0;
}


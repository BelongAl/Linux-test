/*----------------------------------------------
 * 封装实现一个select类，实现一个简单的并发服务器
 * ---------------------------------------------*/
#include<iostream>
#include<vector>
#include<time.h>
#include<sys/select.h>
#include"tcpsocket.hpp"

class select
{
public:
	select():
		m_max_fd(-1),
		FD_ZERO(&m_rfds)
	{}
	
	bool Add(const TcpSocket &socket)//将描述符添加到监控集合
	{
		int fd = socket.GetFd();
		FD_SET(fd, &m_rfds);
		m_maxfd = m_maxfd > fd ? m_maxfd : fd;
		return true;
	}

	bool Del(const TcpSocket &socket)//从监控集合中移除描述符
	{
		int fd = sock.GetFd();
		FD_CLR(fd, &m_rfds);
		if(fd != m_maxfd)
		{
			return true;
		}
		for(int i = fd - 1; i >= 0; i--)//重新判断最大描述符
		{
			if(FD_ISSET(i, &m_rfds))
			{
				m_maxfd = i;
				break;
			}	
		}
		return true;
	}

	bool Wait(std::vector<TcpSocket> &list, int mtimeout = 3000)//开始监控，并开始返回就绪的socket信息
	{
		struct timeval tv;
		tv.tv_usec = (mtimeout % 1000) * 1000;
		tv.tv_sec = mtimtout / 1000;//毫秒
		
		int nfds = select(m_maxfd + 1, *m_rfds, NULL, NULL, &tv);
		if(nfds < 0)
		{
			perror("select error");
			return false;
		}
		else if(nfds == 0)
		{
			printf("wait timeput\n");
			return true;
		}
		for(int i = 0; i < m_maxfd; i++)
		{
			if(FD_ISSET(i, &m_rfds))
			{
				TcpSocket sock;
				sock.SetFd(i);
				list->pushback(sock);
			}
		}
		return true;
	}
	
	

private:
	int m_maxfd;//最大的描述符
	fd_set m_rfds;//可读事件的描述符集合
};

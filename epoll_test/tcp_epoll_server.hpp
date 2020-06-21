/****************************************************
 * 封装一个epoll服务器，只考虑可读事件就绪 
 * belongal
 ****************************************************/
#pragma once

#include<vector>
#include<functional>
#include<sys/epoll.h>
#include<stdio.h>
#include"tcp_socket.hpp"

typedef void(*Handler) (const std::string&, std::string&);

class Epoll{
public:
    Epoll(){
        m_epoll_fd = epoll_create(10);
    } 
    ~Epoll(){
        close(m_epoll_fd);
    }

	//往epoll中添加描述符
    bool Add(const TcpSocket &sock)const {
        int fd = sock.GetFd();
        printf("[EPOLL ADD] fd = %d\n", fd);
        epoll_event ev;
        ev.data.fd = fd;
        ev.events = EPOLLIN;//可读
        int ret = epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &ev);
        if(ret < 0) {
            perror("epoll_ctl ADD");
            return false;
        }
        return true;
    }

    //删除epoll中的描述符
    bool Del(const TcpSocket &sock)const {
        int fd = sock.GetFd();
        printf("[EPOLL DEL] fd = %d\n", fd);
        int ret = epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, fd, NULL);
        if(ret < 0){
            perror("epoll_ctl DEL");
            return false;
        }
        return false;
    }
	
	//获取准备好的描述符
    bool Wait(std::vector<TcpSocket> &output)const {
    	output.clear();
		epoll_event events[1000];
        int nfds = epoll_wait(m_epoll_fd, events, sizeof(events)/sizeof(events[0]), -1);
    	if(nfds < 0){
        	perror("epoll_wait");
			return false;
    	}
		for(int i = 0; i < nfds; ++i) {
			TcpSocket sock(events[i].data.fd);
			output.push_back(sock);
		}
		return true;
    }   

private:
    int m_epoll_fd;
};

class TcpEpollServer {
public:
	TcpEpollServer(const std::string ip, uint16_t port):
				  m_ip(ip),
				  m_port(port){}
	
	bool start(Handler handler){
		//创建套接字
		TcpSocket listen_sock;
	    CHECK_RET(listen_sock.Socket());
		
		//绑定地址信息
		CHECK_RET(listen_sock.Bind(m_ip, m_port));

		//监听		
		CHECK_RET(listen_sock.Listen());
	
		//创建epoll对象，并把listen_sock添加进去
	    Epoll epoll;
		epoll.Add(listen_sock);
		
		//对事件进行循环判断
		while(true){
			std::vector<TcpSocket> output;
			if(!epoll.Wait(output)){
				continue;
			}
			
			//根据文件描述符的种类决定如何处理
			for(size_t i = 0; i < output.size(); ++i){
				if(output[i].GetFd() == listen_sock.GetFd()){
					TcpSocket new_sock;
					CHECK_RET(listen_sock.Accept(new_sock));
					epoll.Add(new_sock);
				}else{
					std::string req, resp;
					bool ret = output[i].Recv(req);
					if(!ret){
						epoll.Del(output[i]);
						output[i].Close();
						continue;
					}
					handler(req, resp);
					output[i].Send(resp);
				}
					
			}				
        }
		return true;
	}
	
private:
	std::string m_ip;
	uint16_t m_port;
};


























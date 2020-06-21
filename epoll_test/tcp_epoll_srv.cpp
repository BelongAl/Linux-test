#include <stdlib.h>
#include "tcp_epoll_server_test.hpp"

void handler(const std::string &req, std::string &resp){
	std::cout << "clint say: " << req <<std::endl;
	std::cin >> resp;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cout << "plase scanf ./tcp_srv.cpp ip port\n";
        return -1;
    }
	std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

	std::cout << "ip: " << ip <<std::endl;
	std::cout << "port: " << port <<std::endl;
	
	TcpEpollServer lst_sock(ip, port);
	lst_sock.Start(handler);

	return 0;
}

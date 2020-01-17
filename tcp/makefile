all::tcp_srv tcp_cli process_tcp thread_tcp
thread_tcp:thread_tcp.cpp
	g++ thread_tcp.cpp  -o thread_tcp -lpthread
tcp_srv:tcp_srv.cpp
	g++ $^ -o $@
tcp_cli:tcp_cli.cpp
	g++ $^ -o $@
process_tcp:process_tcp.cpp
	g++ $^ -o $@

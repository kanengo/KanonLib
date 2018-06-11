#include <cstdio>
#include <iostream>
#include "netLib/Socket.h"
int main()
{	
	Socket s = Socket::socket(AF_INET, SOCK_STREAM, 0);
	s.bind(54167);
	s.listen(1024);
	while (true) {
		Socket sc = s.accept();
		std::cout << "accept!!!" << std::endl;
		InetAddr addr = sc.getInetAddr();
		std::cout << "clien addr:" << addr.getIp() << ",port:" << addr.getPort() << std::endl;
		
	}
    printf("hello from KanonLib!\n");
    return 0;
}
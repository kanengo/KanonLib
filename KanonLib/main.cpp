#include <cstdio>
#include <iostream>
#include "netLib/Socket.h"
#include <thread>
using namespace std;
int main()
{	
	try {
		Socket s = Socket::socket(AF_INET, SOCK_STREAM, 0);
		/*s.bind(54167);
		s.listen(1024);
		
		while (true) {
			Socket sc = s.accept();
			std::cout << "accept!!!" << std::endl;
		}*/
		s.connect("127.0.0.1", 13003);
		cout << "connect success" << endl;
		while (true);
	}
	catch (Socket::SocketException &e) {
		std::cout << e.description() << std::endl;
	}
    return 0;
}
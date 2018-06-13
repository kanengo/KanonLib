#include "Epoll.h"



Epoll::Epoll()
{
	_efd = epoll_create(1024);
	if (_efd == -1) {
		throw SocketException("create socket error:" + std::string(strerror(errno)));
	}
}


Epoll::~Epoll()
{
}

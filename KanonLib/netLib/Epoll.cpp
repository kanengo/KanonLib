#include "Epoll.h"



Epoll::Epoll()
{
	_efd = epoll_create(1024);
	if (_efd == -1) {
		throw SocketException("create socket error:" + std::string(strerror(errno)));
	}
}

void Epoll::addEvent(int fd, epoll_event * event)
{
	if (epoll_ctl(_efd, EPOLL_CTL_ADD, fd, event) == -1) {
		throw SocketException("epoll addEvent error:" + std::string(strerror(errno)));
	}
}

void Epoll::modEvent(int fd, epoll_event * event)
{
	if (epoll_ctl(_efd, EPOLL_CTL_MOD, fd, event) == -1) {
		throw SocketException("epoll addEvent error:" + std:	:string(strerror(errno)));
	}
}

void Epoll::delEvent(int fd, epoll_event * event)
{
	if (epoll_ctl(_efd, EPOLL_CTL_DEL, fd, event) == -1) {
		throw SocketException("epoll addEvent error:" + std::string(strerror(errno)));
	}
}

void Epoll::addAcceptFd(int fd)
{
	_sockfd = fd;
}



Epoll::~Epoll()
{
	if (_efd > 0)
		close(_efd);
}

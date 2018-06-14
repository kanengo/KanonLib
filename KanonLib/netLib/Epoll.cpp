#include "Epoll.h"



Epoll::Epoll()
{
	_efd = epoll_create(1024);
	if (_efd == -1) {
		throw SocketException("create socket error:" + std::string(strerror(errno)));
	}
}

void Epoll::del(int fd)
{
	if (epoll_ctl(_efd, EPOLL_CTL_ADD, fd, nullptr) == -1) {
		throw SocketException("epoll addEvent error:" + std::string(strerror(errno)));
	}
}

void Epoll::add(int fd, epoll_event * event)
{
	if (epoll_ctl(_efd, EPOLL_CTL_ADD, fd, event) == -1) {
		throw SocketException("epoll addEvent error:" + std::string(strerror(errno)));
	}
}

void Epoll::mod(int fd, epoll_event * event)
{
	if (epoll_ctl(_efd, EPOLL_CTL_MOD, fd, event) == -1) {
		throw SocketException("epoll addEvent error:" + std::string(strerror(errno)));
	}
}

void Epoll::wait(epoll_event * events, int maxevents, int timeout = -1)
{
	::epoll_wait(_efd, events, maxevents, timeout);
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

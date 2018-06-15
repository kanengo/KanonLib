#include "Epoll.h"



Epoll::Epoll(int maxevents = 64)
{
	_efd = epoll_create(1024);
	if (_efd == -1) {
		throw SocketException("create socket error:" + std::string(strerror(errno)));
	}
	_maxevents = maxevents;
	_events = new epEvent[maxevents];
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

int Epoll::wait(epoll_event * events, int maxevents, int timeout = -1)
{
	return ::epoll_wait(_efd, events, maxevents, timeout);
}

void Epoll::addListenFd(int fd)
{
	_listenfd = fd;
}

void Epoll::modEvnet(int fd, void * data, bool write)
{
	epEvent ev;
	ev.data.ptr = data;
	ev.events = EPOLLET | EPOLLIN | EPOLLERR | EPOLLRDHUP | EPOLLHUP|(write ? EPOLLOUT : 0);
	mod(fd, &ev);
}

void Epoll::setOnConnectCallback(callback cb)
{
	_onConnectCallback = cb;
}

void Epoll::setOnReadCallback(callback cb)
{
	_onReadCallback = cb;
}

void Epoll::setWriteCallback(callback cb)
{
	_onWriteCallback = cb;
}

void Epoll::poll()
{
	int nevent;
	while (true)
	{
		nevent = wait(_events, _maxevents);
		for (int i = 0; i < nevent; i++)
		{
			auto data = static_cast<SocketData* >(_events[i].data.ptr);
			if (data->fd == _listenfd) {
				int  listenfd = data->fd;
				Socket s(listenfd);
				sockaddr_in clientaddr;
				Socket newSock = s.accept(&clientaddr);
				InetAddr addr(clientaddr);
				SocketChannle sc(newSock, addr);

				if (_onConnectCallback) {
					_onConnectCallback(sc);
				}
			}
			else if(_events[i].events & EPOLLIN){

			}
		}
	}
}


void Epoll::addEvent(int fd, void* data)
{
	epEvent ev;
	ev.data.ptr = data;
	ev.events = EPOLLET | EPOLLIN | EPOLLERR | EPOLLRDHUP | EPOLLHUP;
	add(fd, &ev);
}



Epoll::~Epoll()
{
	if (_efd > 0)
		close(_efd);
	if(_events)
		delete []_events;
}

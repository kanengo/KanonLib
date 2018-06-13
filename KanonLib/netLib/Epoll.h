#pragma once
#include <iostream>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <functional>

#include "SocketException.h"
class Epoll
{
	typedef std::function<void()> callback;
public:
	Epoll();
	void addEvent(int fd, epoll_event *event);
	void modEvent(int fd, epoll_event *event);
	void delEvent(int fd, epoll_event *event);
	void wait(epoll_event *events, int maxevents, int timeout);

	void addAcceptFd(int fd);
	void addReadEvent(int fd);

	void setOnConnectCallback(callback cb);
	void setOnreadCallback(callback cb);
	void setWriteCallback(callback cb);
	void poll();
	virtual ~Epoll();
private:
	int _efd;
	int _sockfd;
	
	callback _onConnectCallback;
	callback _onReadCallback;
	callback _onWriteCallback;

};


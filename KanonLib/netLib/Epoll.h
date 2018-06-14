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
	typedef epoll_event epEvent;
public:
	Epoll();
	void add(int fd, epEvent *event);
	void mod(int fd, epEvent *event);
	void del(int fd);
	void wait(epEvent *events, int maxevents, int timeout);

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


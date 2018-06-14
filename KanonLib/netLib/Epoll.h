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
#include "PollData.h"
#include "Socket.h"
#include "InetAddr.h"
#include "SocketChannel.h"
class Epoll
{
	typedef std::function<void(SocketChannle&)> callback;
	typedef epoll_event epEvent;
public:
	Epoll(int maxevents);
	void add(int fd, epEvent *event);
	void mod(int fd, epEvent *event);
	void del(int fd);
	int wait(epEvent *events, int maxevents, int timeout);

	void addAcceptFd(int fd);
	void addEvent(int fd, void* data);
	void modEvnet(int fd, void* data, bool write);
	
	void setOnConnectCallback(callback cb);
	void setOnReadCallback(callback cb);
	void setWriteCallback(callback cb);
	void poll();
	virtual ~Epoll();
private:
	int _efd;
	int _sockfd;
	int _maxevents;
	epEvent *_events;
	
	
	callback _onConnectCallback;
	callback _onReadCallback;
	callback _onWriteCallback;
	
};


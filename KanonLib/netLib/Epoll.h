#pragma once
#include <iostream>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
class Epoll
{
public:
	Epoll();
	void addEvent();
	void del();
	void modify();
	virtual ~Epoll();

};


#pragma once

#include "Socket.h"
#include "InetAddr.h"

class SocketChannle
{
public:
	SocketChannle(Socket &s, InetAddr &addr);
	int getSockFd();
	const Socket & getSocket() const;
	const InetAddr & getInetAddr() const;
private:
	Socket _sock;
	InetAddr _iaddr;
};


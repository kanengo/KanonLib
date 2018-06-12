#include "SocketChannel.h"

SocketChannle::SocketChannle(Socket & s, InetAddr & addr)
	:_sock(s), _iaddr(addr)
{
}

int SocketChannle::getSockFd()
{
	return _sock.getFd();
}

const Socket & SocketChannle::getSocket() const
{
	return _sock;
}

const InetAddr & SocketChannle::getInetAddr() const
{
	return _iaddr;
}

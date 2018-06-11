#include "InetAddr.h"

InetAddr::InetAddr(std::string & ip, int port)
	:_ip(ip),_port(port)
{
	
}

InetAddr::InetAddr(const sockaddr_in & addr)
{	
	char ipc_str[16];
	_ip = inet_ntop(AF_INET, &addr.sin_addr, ipc_str, INET_ADDRSTRLEN);
	_port = ntohs(addr.sin_port);
}

std::string InetAddr::getIp() const
{
	return _ip;
}

int InetAddr::getPort() const
{
	return _port;
}

InetAddr::~InetAddr()
{

}

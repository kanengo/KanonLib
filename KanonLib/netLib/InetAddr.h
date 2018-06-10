#pragma once
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
class InetAddr
{
public:
	InetAddr(std::string &ip, int port);
	InetAddr(const sockaddr_in &addr);
	std::string getIp() const;
	int getPort() const;
	~InetAddr();
private:
	std::string _ip;
	int _port;
};


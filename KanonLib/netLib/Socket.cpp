#include "Socket.h"


Socket Socket::socket(int protofamily = AF_INET, int type = SOCK_STREAM, int protocol = 0) {
	int sockfd = ::socket(protofamily, type, protocol);
	if (sockfd <= 0) {
		throw Socket::SocketException("create socket error:" + std::string(strerror(errno)));
	}
	
	return Socket(sockfd);
}

void Socket::setsockopt(int level, int optname, int optval)
{
	if (::setsockopt(sock_fd, level, optname, &optval, static_cast<socklen_t>(sizeof(optval))) == -1) {
		throw Socket::SocketException("socket setsockopt error:" + std::string(strerror(errno)));
	}
}

void Socket::setsockopt(int level, int optname, const void *optval, socklen_t optsize)
{
	if (::setsockopt(sock_fd, level, optname, optval, optsize) == -1) {
		throw Socket::SocketException("socket setsockopt error:" + std::string(strerror(errno)));
	}
}

bool Socket::bind(const int port)
{
	addrinfo ai_hints;
	addrinfo *ai_res = nullptr;
	ai_hints.ai_family = AF_UNSPEC;
	ai_hints.ai_protocol = IPPROTO_TCP;
	ai_hints.ai_socktype = SOCK_STREAM;
	ai_hints.ai_flags = AI_PASSIVE;
	char strPort[6];
	sprintf(strPort, "%d", port);
	if (getaddrinfo("0.0.0.0", strPort, &ai_hints, &ai_res) != 0) {
		throw Socket::SocketException("function 'getaddrinfo' error:" + std::string(strerror(errno)));
	}
	if (::bind(sock_fd, ai_res->ai_addr, sizeof(sockaddr)) != 0) {
		throw Socket::SocketException("socket bind error:" + std::string(strerror(errno)));
	}

	return true;
}

bool Socket::listen(const int maxListenNum)
{
	if (::listen(sock_fd, maxListenNum) != 0) {
		throw Socket::SocketException("socket listen error:" + std::string(strerror(errno)));
	}
	return true;
}

bool Socket::setblocking(const bool isBlock)
{
	int flag = fcntl(sock_fd, F_GETFL, 0);
	if (isBlock)
		flag |= O_NONBLOCK;
	else
		flag &= O_NONBLOCK;
	if (fcntl(sock_fd, F_SETFL, flag) == -1) {
		throw Socket::SocketException("set socket blocking error:" + std::string(strerror(errno)));
	}

	return true;
}

bool Socket::connect(const std::string host, const int port)
{
	return false;;
}

InetAddr Socket::getInetAddr()
{
	return _addr;
}

Socket Socket::accept()
{
	sockaddr_in client_addr;
	socklen_t socklen = static_cast<socklen_t>(sizeof(sockaddr));
	int client_fd = ::accept(sock_fd, (sockaddr*)&client_addr, &socklen);
	if (client_fd == -1) {
		throw Socket::SocketException("socket accept error:" + std::string(strerror(errno)));
	}

	InetAddr addr(client_addr);
	Socket s(client_fd, addr);
	
	return s;
}

bool Socket::close()
{
	if (sock_fd > 0)
		::close(sock_fd);
	else
		return false;
	sock_fd = -1;
	return true;
}


Socket::~Socket()
{
	close();
}

bool Socket::isVaild()
{
	if (sock_fd <= 0)
		return false;
	return true;
}

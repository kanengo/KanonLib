#include "Socket.h"


Socket Socket::socket(int protofamily = AF_INET, int type = SOCK_STREAM, int protocol = 0) {
	int sockfd = ::socket(protofamily, type, protocol);
	if (sockfd <= 0) {
		throw SocketException("create socket error:" + std::string(strerror(errno)));
	}
	
	return Socket(sockfd);
}

void Socket::setsockopt(int level, int optname, int optval)
{
	if (::setsockopt(sock_fd, level, optname, &optval, static_cast<socklen_t>(sizeof(optval))) == -1) {
		throw SocketException("socket setsockopt error:" + std::string(strerror(errno)));
	}
}

void Socket::setsockopt(int level, int optname, const void *optval, socklen_t optsize)
{
	if (::setsockopt(sock_fd, level, optname, optval, optsize) == -1) {
		throw SocketException("socket setsockopt error:" + std::string(strerror(errno)));
	}
}

bool Socket::bind(const uint16_t port)
{
	addrinfo ai_hints;
	addrinfo *ai_res = nullptr;
	ai_hints.ai_family = AF_UNSPEC;
	ai_hints.ai_protocol = IPPROTO_TCP;
	ai_hints.ai_socktype = SOCK_STREAM;
	ai_hints.ai_flags = AI_PASSIVE;
	//char strPort[6];
	//sprintf(strPort, "%d", port);
	if (getaddrinfo("0.0.0.0", std::to_string(port).c_str(), &ai_hints, &ai_res) != 0) {
		throw SocketException("function 'getaddrinfo' error:" + std::string(strerror(errno)));
	}
	if (::bind(sock_fd, ai_res->ai_addr, sizeof(sockaddr)) != 0) {
		throw SocketException("socket bind error:" + std::string(strerror(errno)));
	}

	return true;
}

bool Socket::listen(const int maxListenNum)
{
	if (::listen(sock_fd, maxListenNum) != 0) {
		throw SocketException("socket listen error:" + std::string(strerror(errno)));
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
		throw SocketException("set socket blocking error:" + std::string(strerror(errno)));
	}

	return true;
}

bool Socket::connect(const std::string host, const uint16_t port)
{	
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	inet_pton(AF_INET, host.c_str(), &serveraddr.sin_addr);
	if (::connect(sock_fd, (sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
		throw SocketException("socket connect error:" + std::string(strerror(errno)));
	}
	return true;
}

ssize_t Socket::recv(void * buf, size_t sz, int flag = 0)
{
	//size_t left = sz;
	//ssize_t nread = 0;
	//char* p = static_cast<char *>(buf);
	//while (left > 0) {
	//	nread = ::recv(sock_fd, p, left, flag);
	//	if (nread == 0) {
	//		break;
	//	}
	//	else if (nread < 0) {
	//		if (errno == EINTR)
	//			continue;
	//		return -1;
	//	}
	//	else {
	//		left -= nread;
	//		p += nread;
	//	}
	//}
	//
	//return sz - left;
	ssize_t nread = ::recv(sock_fd, buf, sz, flag);
	return nread;
	
}

ssize_t Socket::send(const void * buf, size_t len, int flag = 0)
{
	return ::send(sock_fd, buf, len, flag);
}

Socket Socket::accept(sockaddr_in* clientaddr = nullptr)
{
	socklen_t socklen = static_cast<socklen_t>(sizeof(sockaddr));
	int client_fd = ::accept(sock_fd, (sockaddr*)clientaddr, &socklen);
	if (client_fd == -1) {
		throw SocketException("socket accept error:" + std::string(strerror(errno)));
	}

	Socket s(client_fd);
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

int Socket::getFd() const
{
	return sock_fd;
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

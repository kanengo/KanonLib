#pragma once
#include <sys/socket.h>
#include <cerrno>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "InetAddr.h"

class Socket
{
public:
	class SocketException;
	Socket(int sockfd) :sock_fd(sockfd) {};
	static Socket socket(int protofamily, int type, int protocol);
	void setsockopt(int level, int optname, int optval);
	void setsockopt(int level, int optname, const void *optval, socklen_t optsize);
	bool bind(const uint16_t port);
	bool listen(const int maxListenNum);
	bool setblocking(const bool isBlock);
	bool connect(const std::string host, const uint16_t port);
	ssize_t recv(void* buf, size_t len, int flag);
	ssize_t send(const void *buf, size_t len, int flag);
	Socket accept();
	bool close();
	int getFd() const;
	~Socket();

public:
	class SocketException {
	public:
		SocketException(std::string msg):_msg(msg),_errorCode(errno){};
		std::string description() {
			return _msg;
		}
		int getErrorCode() {
			return _errorCode;
		}
	private:
		std::string _msg;
		int _errorCode;
	};
private:
	bool isVaild();
	int sock_fd;
};


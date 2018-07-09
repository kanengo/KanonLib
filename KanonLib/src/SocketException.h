#pragma once
#include <string>

class SocketException{
public:
	SocketException() :_errorCode(errno) {};
	SocketException(std::string msg) :_msg(std::move(msg)), _errorCode(errno) {};
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
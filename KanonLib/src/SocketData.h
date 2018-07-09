#pragma once

class SocketData
{
public:
	SocketData(int fd) :fd(fd) {};
public:
	int fd;
};
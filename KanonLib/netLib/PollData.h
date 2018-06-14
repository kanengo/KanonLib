#pragma once

class PollData
{
public:
	PollData(int fd) :fd(fd) {};
public:
	int fd;
	void *data;
};
#pragma once

#include <atomic>
#include <vector>
#include <list>
#include <array>

using namespace std;

template<class T>
class SynchronizedQueue
{
public:
	
	SynchronizedQueue(int size, int cacheSize);
	void push(T &data);
	T pop();
private:
	class QueueData {
	public:
		static const uint8_t WRITE = 1;
		static const uint8_t READ = 2;
		
		T data;
		QueueData() {
			flag = WRITE;
		}
		uint8_t flag;
	};
private:
	vector<QueueData> _q;
	list<T> _qc;
	int _size;
	int _cachesize;
	volatile int _rIdx;
	volatile int _wIdx;
	atomic_flag _lock = ATOMIC_FLAG_INIT;

	void lock();
	void unlock();
};

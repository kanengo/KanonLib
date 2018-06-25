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

	
private:
	vector<T> _q;
	
	list<T> _qc;
	int _size;
	int _cachesize;
	volatile int _rIdx;
	volatile int _wIdx;
	atomic_flag _lock = ATOMIC_FLAG_INIT;

	void lock()
	{
		while (_lock.test_and_set(memory_order_acquire));
	}

	void unlock() {
		_lock.clear();
	}
};

template<class T>
inline SynchronizedQueue<T>::SynchronizedQueue(int size, int cacheSize)
{
}

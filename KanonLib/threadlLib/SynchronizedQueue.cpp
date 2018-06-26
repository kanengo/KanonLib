
#include "SynchronizedQueue.h"

template<class T>
SynchronizedQueue<T>::SynchronizedQueue(int size = 1024, int cacheSize = 0) 
	:_size(size), _cachesize(cacheSize), _q(vector<QueueData>(_size, QueueData())),_qc(list<T>())
{	
	_rIdx = 0;
	_wIdx = 0;
}

template<class T>
void SynchronizedQueue<T>::push(T &data)
{	
	lock();
	auto &qd = _q[_wIdx];
	if (qd.flag == QueueData::WRITE) {
		qd.data = data;
		_wIdx = (_wIdx + 1) % _size;
		qd.flag = QueueData::READ;
	}
	else {
		if (_qc.size() < _cachesize) {
			_qc.push_back(data);
		}
	}
	unlock();
}

template<class T>
T SynchronizedQueue<T>::pop()
{
	auto &qd = _q[_rIdx];
	lock();
	if (qd.flag == QueueData::READ) {
		qd.flag = QueueData::WRITE;
		_rIdx = (_rIdx + 1) % _size;
		unlock();
		return qd.data;
	}
	unlock();
	return T();
}

template<class T>
void SynchronizedQueue<T>::lock()
{
	while (_lock.test_and_set(memory_order_acquire));
}

template<class T>
void SynchronizedQueue<T>::unlock() {
	_lock.clear();
}
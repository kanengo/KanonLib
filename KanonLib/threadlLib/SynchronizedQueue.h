#pragma once

#include <atomic>
#include <vector>
#include <list>

using namespace std;

template<class T>
class SynchronizedQueue
{
public:
	SynchronizedQueue(int size, int cacheSize):_size(size), _cachesize(cacheSize) {
		vector<T> queue(size);
		_queue = &q;
		
	}

	
private:
	vector<T> *_q;
	list<T> _qc;
	int _size;
	int _cachesize;
};
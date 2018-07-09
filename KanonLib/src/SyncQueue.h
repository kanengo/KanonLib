#pragma once
#include <atomic>
#include <vector>
#include <list>
#include <condition_variable>
#include "Lock.h"

using namespace std;

template<typename T>
class SyncQueue
{
public:
	static const uint8_t WRITE = 1;
	static const uint8_t READ = 2;
private:
	
	class QueueData {
	public:
		T data;
		uint8_t flag;
		QueueData() {
			flag = SyncQueue::WRITE;
		};
	};
	
public:
	SyncQueue(int size = 1024, int cacheSize = 0)
		:m_capacity(size), m_cachesize(cacheSize)
	{
		m_count = 0;
		m_rIdx = 0;
		m_wIdx = 0;
		m_q = new QueueData*[m_capacity];
		for (int i = 0; i < m_capacity; i++)
			m_q[i] = new QueueData();
		//m_q = new QueueData[m_capacity];
	}

	~SyncQueue()
	{
		for (int i = 0; i < m_capacity; i++)
			delete m_q[i];
		delete[] m_q;
		//delete m_q;
	}
	
	bool push_and_wait(const T & ref) {
		LockGuard<Lock> lg(m_lock);
		while (full()) {
			cvfull.wait(m_lock);
		}
		_push(ref);
		return true;
	}

	bool push(const T & ref) 
	{
		LockGuard<Lock> lg(m_lock);
		auto ret = _push(ref);
		return ret;
		
	}

	T pop_and_wait() {
		LockGuard<Lock> lg(m_lock);
		while (empty()) {
			cvempty.wait(m_lock);
		}
		//auto ret = _pop();
		
		return std::ref(_pop());
	}

	T pop() {
		LockGuard<Lock> lg(m_lock);
		T &ret = _pop();
		return ret;
	}

	bool empty() const {
		return m_q[m_rIdx]->flag == SyncQueue::WRITE;
	}

	bool full() const {
		return m_q[m_wIdx]->flag == SyncQueue::READ;
	}
private:

	bool _push(const T & ref) {
		if (!full()) {
			QueueData* node = m_q[m_wIdx];
			node->flag = SyncQueue::READ;
			node->data = ref;
			m_wIdx = (m_wIdx + 1) % m_capacity;
			m_count += 1;
			cvempty.notify_all();
			return true;
		}
		/*else if(m_qc.size() < m_cachesize){
			m_qc.push_back(ref);
		}*/

		return false;
	}

	T& _pop() {
		if (empty())
			cerr << "not value reutrn !!!";
		QueueData * node = m_q[m_rIdx];
		node->flag = SyncQueue::WRITE;
		m_rIdx = (m_rIdx + 1) % m_capacity;
		m_count -= 1;
	/*	if (!m_qc.empty()) {
			_push(m_qc.front());
			m_qc.pop_front();
		}*/
		cvfull.notify_all();
		return node->data;

	}

	int m_capacity;
	int m_cachesize;
	QueueData ** m_q;
	//vector<QueueData> m_q;
	//list<T> m_qc;
	volatile int m_rIdx;
	volatile int m_wIdx;
	volatile int m_count;
	condition_variable_any cvempty;
	condition_variable_any cvfull;
	Lock m_lock;

};


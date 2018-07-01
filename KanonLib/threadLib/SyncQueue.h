#pragma once
#include <atomic>
#include <vector>
#include <list>
#include <condition_variable>

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

	class Lock {
	public:
		void lock() {
			while (m_lock.test_and_set(memory_order_acquire));
		}
		void unlock() {
			m_lock.clear();
		}
	private:
		atomic_flag m_lock = ATOMIC_FLAG_INIT;
	};
	
public:
	SyncQueue(int size = 1024, int cacheSize = 0)
		:m_capacity(size), m_cachesize(cacheSize), m_qc(list<T>())
	{
		m_count = 0;
		m_rIdx = 0;
		m_wIdx = 0;
		m_q = new QueueData[m_capacity];
	}

	~SyncQueue()
	{
		delete m_q;
	}
	
	bool push_and_wait(const T & ref) {
		m_lock.lock();
		while (full()) {
			cvfull.wait(m_lock);
		}
		_push(ref);
		m_lock.unlock();
		cvempty.notify_all();
		return true;
	}

	bool push(const T & ref) 
	{
		m_lock.lock();
		auto ret = _push(ref);
		m_lock.unlock();
		return ret;
		
	}

	T& pop_and_wait() {
		m_lock.lock();
		while (empty()) {
			cvempty.wait(m_lock);
		}
		T &ret = _pop();
		m_lock.unlock();
		cvfull.notify_all();
		return ret;
	}

	T& pop() {
		m_lock.lock();
		T &ret = _pop();
		m_lock.unlock();
		return ret;
	}

	bool empty() const {
		return m_q[m_rIdx].flag == SyncQueue::WRITE;
	}

	bool full() const {
		return m_q[m_wIdx].flag == SyncQueue::READ;
	}
private:

	bool _push(const T & ref) {
		if (!full()) {
			if (ref->counter > 0) {
				cerr << "push counter > 0 1111111111111111111111~~~~~~~~~~~~~~~~~~~~~~" << ref->counter << ",m_wIdx:" << m_wIdx << ",m_rIdx:" << m_rIdx << endl;
			}
			auto rt = ref;
			auto flag = m_q[m_wIdx].flag;
			auto nt = m_q[m_wIdx];
			auto & node = m_q[m_wIdx];
			node.flag = SyncQueue::READ;
			node.data = ref;
			m_wIdx = (m_wIdx + 1) % m_capacity;
			m_count += 1;
			if (node.data->counter > 0) {
				cerr << "push counter > 0 2222222222222222222222222~~~~~~~~~~~~~~~~~~~~~~" << node.data->counter << ",m_wIdx:" << m_wIdx << ",m_rIdx:" << m_rIdx << endl;
			}
			return true;
		}
		else if(m_qc.size() < m_cachesize){
			m_qc.push_back(ref);
		}

		return false;
	}

	T& _pop() {
		if (empty())
			cerr << "not value reutrn !!!";

		auto &node = m_q[m_rIdx];
		node.flag = SyncQueue::WRITE;
		m_rIdx = (m_rIdx + 1) % m_capacity;
		m_count -= 1;
		if (!m_qc.empty()) {
			_push(m_qc.front());
			m_qc.pop_front();
		}
		if (node.data->counter > 0);
			//cerr << "counter > 0!!!!!!!!!!!!!!!!!!!!!!!!!!!@@@@@@@@@@@@" << endl;
		return node.data;

	}

	int m_capacity;
	int m_cachesize;
	QueueData * m_q;
	list<T> m_qc;
	volatile int m_rIdx;
	volatile int m_wIdx;
	volatile int m_count;
	condition_variable_any cvempty;
	condition_variable_any cvfull;
	Lock m_lock;

};


#pragma once

#include <atomic>

class Lock {
public:
	void lock() {
		while (m_lock.test_and_set(std::memory_order_acquire));
	}
	void unlock() {
		m_lock.clear();
	}
private:
	std::atomic_flag m_lock = ATOMIC_FLAG_INIT;
};

template<class T>
class LockGuard {
public:
	LockGuard(T &lock) :m_lock(lock) {
		m_lock.lock();
	}

	~LockGuard() {
		m_lock.unlock();
	}

private:
	T & m_lock;
};

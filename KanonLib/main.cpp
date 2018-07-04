#include <cstdio>
#include <iostream>
#include "netLib/KanonNet.h"
#include <thread>
#include "threadLib/SyncQueue.h"
#include <atomic>
#include <unistd.h>
#include <map>
#include <stdlib.h>
#include "threadLib/Lock.h"
#include <memory>

using namespace std;

string task(string msg)
{
	cout << msg << endl;
	return msg;
}

class Test
{
public:
	Test() {
		cout << "class test ctor"<<endl;
	}
	Test(int t) {
		cout << "class test ctor iiiii" << endl;;
	}
};

class A
{
public:
	A(int size = 10, int data = 0, long long id = 0)
	{	
		// cout << "A ctor " << endl;
		ptr = new int[size];
		memset(ptr, 0, size);
		for (int i = 0; i < size; i++)
			*(ptr + i) = data;
		m_size = size;
		m_id = id;
		flag = false;
		counter = 0;
	};
	A(const A & rhs) {
		cout << "A copy ctor" <<endl;
		ptr = new int[rhs.m_size];
		m_size = rhs.m_size;
		for (int i = 0; i < m_size; i++) {
			*(ptr + i) = *(rhs.ptr + i);
		}
		m_id = rhs.m_id;
	}

	A(A&& rhs) {
		cout << "A rvalue cpoy ctor" << endl;
		swap(ptr, rhs.ptr);
		m_size = rhs.m_size;
		m_id = rhs.m_id;
	}

	A& operator=(A&& rhs) {
		cout << "A rvalue copy ctor=======" << endl;
		swap(ptr, rhs.ptr);
		m_size = rhs.m_size;
		m_id = rhs.m_id;
		return *this;
	}
	
	A& operator=(const A & rhs) {
		cout << "A copy ctor=======" << endl;
		if (this == &rhs)
			return *this;
		if (ptr != nullptr)
			delete[] ptr;
		ptr = new int[rhs.m_size];
		m_size = rhs.m_size;
		for (int i = 0; i < m_size; i++) {
			*(ptr + i) = *(rhs.ptr + i);
		}
		m_id = rhs.m_id;
		//delete[] oldptr;
		return *this;
	}

	~A() {
		delete[] ptr;
	}
	
	int *ptr;
	int m_size;
	long long m_id;
	bool flag;
	int counter;
};

int main()
{
	SyncQueue<shared_ptr<A>> queue(65536);
	vector<thread> threads;
	atomic_llong readcount;
	atomic_llong writecount ;
	atomic_llong incId;
	incId = 0;
	readcount = 0;
	writecount = 0;
	Lock lock;
	for (int i = 0; i < 2; i++) {
		threads.emplace_back([&](int idx) ->void {
			int i = 0;
			while (true) {
				auto p = make_shared<A>(10, 42, incId++);
				queue.push_and_wait(p);
				//usleep(1);
				writecount += 1;
				// i++;
			}	
		}, i);
	}

	map<int, bool> m;
	for (int i = 0; i < 6; i++) {
		threads.emplace_back([&]()->void {
			
			while (true) {
				auto ret = queue.pop_and_wait();
				//lock.lock();
				if (ret->counter > 0) {
					cout << "funck!!!!!!!!  " << ret->m_id <<"counter:"<< ret->counter  <<endl;
					//lock.unlock();
					//break;
				}
				
				//else {
				ret->counter++;
				ret->flag = true;
				//}
				//lock.unlock();
	/*			lock.lock();
				if (m.find(ret->m_id) != m.end()){
					cout << "funck!!!!!!!!  " << ret->m_id << endl;
					lock.unlock();
					break;
				}*/
				//m[ret->m_id] = true;
				//lock.unlock();
				//delete ret;
				usleep(1);
				readcount += 1;
			}
		});
	}

	//for (int i = 0; i < threads.size(); i++) {
	//	threads[i].detach();
	//}


	while (true) {
		usleep(1000000);
		cout << "read count:" << readcount << ",writecount"<< writecount<< endl;
		readcount = 0;
		writecount = 0;
	}

	//for (int i = 0; i <threads.size(); i++) {
	//	if (threads[i].joinable())
	//		threads[i].join();
	//}

	cout << sizeof(queue) << endl;
	cout << "-------------------------" << endl;
	//while (1);
    return 0;
}
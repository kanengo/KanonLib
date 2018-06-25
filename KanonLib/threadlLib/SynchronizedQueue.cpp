
#include "SynchronizedQueue.h"

template<class T>
SynchronizedQueue<T>::SynchronizedQueue(int size = 1024, int cacheSize = 0) 
	:_size(size), _cachesize(cacheSize), _q(vector(_size)) 
{

}
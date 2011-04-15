#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include <iostream>
#include "MemoryContainer.hpp"

using namespace std;

template<typename T>
class Memory {
	static MemoryContainer& getContainer() {
		return MemoryContainer::get();
	}

	T data;

public:
	void* operator new(size_t size) {
		return getContainer().allocate(sizeof(Memory<T>));
	}

	void operator delete(void* m) {
		getContainer().free(m);
	}

	void operator=(T d) {
		data = d;
	}

	T get() {
		return data;
	}

	int operator<(Memory<T> right) {
		return data < right.get();
	}
};

template<typename T>
ostream& operator<<(ostream& o, Memory<T>& m) {
	o << m.get();
	return o;
}

#endif /* MEMORY_HPP_ */

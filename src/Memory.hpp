#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include <iostream>
#include "MemoryContainer.hpp"

using namespace std;

/**
 * Dynamic memory.
 *
 * Holds a single instance of a type given in
 * the template.
 * Able to use it's internal MemoryContainer to
 * allocate dynamic memory via the redefined
 * new and delete operators.
 */
template<typename T>
class Memory {
	/**
	 * Gets the MemoryContainer via it's
	 * singleton interface. Encapsulates
	 * external object logic.
	 *
	 * @return The memory container
	 */
	static MemoryContainer& getContainer() {
		return MemoryContainer::get();
	}

	/**
	 * The dynamic object of type specified
	 * int the template.
	 */
	T data;

public:
	/**
	 * Allocates memory in the container
	 * of size of the templated Memory object.
	 *
	 * @param size Memory size to allocate - unused.
	 * @return Address of the allocated memory.
	 */
	void* operator new(size_t size) {
		return getContainer().allocate(sizeof(Memory<T>));
	}

	/**
	 * Frees the previously allocated memory.
	 *
	 * @param m Pointer to the memory address to free
	 */
	void operator delete(void* m) {
		getContainer().free(m);
	}

	/**
	 * Copies the given value to the
	 * internal storage.
	 *
	 * @param d Value to copy into
	 */
	void operator=(T d) {
		data = d;
	}

	/**
	 * Gets the stored value
	 *
	 * @return The stored value
	 */
	T get() {
		return data;
	}

	/**
	 * Compares two Memory object of the same
	 * template type.
	 *
	 * @param right Righthand operand of <
	 * @see The stored type's operator<
	 * @return positive: left greater,
	 * 0: equality,
	 * negative: right greater
	 */
	int operator<(Memory<T> right) {
		return data < right.get();
	}
};


/**
 * Global funtion to be able to
 * print out any Memory object instantiated by
 * arbitrary template type, as the internally
 * stored object has been written out.
 *
 * @param o Output stream to write
 * @param m Memory object to write out
 * @see Memory::get()
 */
template<typename T>
ostream& operator<<(ostream& o, Memory<T>& m) {
	o << m.get();
	return o;
}

#endif /* MEMORY_HPP_ */

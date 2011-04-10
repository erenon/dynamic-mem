/*
 * MemoryContainer.hpp
 *
 *  Created on: 2011.04.10.
 *      Author: erenon
 */

#ifndef MEMORYCONTAINER_HPP_
#define MEMORYCONTAINER_HPP_

#include <cstddef>

#define MEMORY_SIZE (1 << 10)
#define MAX_ALLOC_COUNT 20

class MemoryContainer {
	static MemoryContainer instance;

	char *memory;
	int current_alloc_count;

	void* alloc_address[MAX_ALLOC_COUNT];
	size_t alloc_size[MAX_ALLOC_COUNT];

	MemoryContainer() {
		memory = new char[MEMORY_SIZE];
		current_alloc_count = 0;

		for (int i = 0; i < MAX_ALLOC_COUNT; i++) {
			alloc_address[i] = NULL;
			alloc_size[i] = 0;
		}

		instance = *this;
	}

	size_t getAllocSizeAtAddr(char* addr) {
		size_t size = 0;

		for (int i = 0; i < MAX_ALLOC_COUNT; i++) {
			if (alloc_address[i] == addr) {
				size = alloc_size[i];
				break;
			}
		}

		return size;
	}

	void *getFreeAddress(size_t requested_size) {
		size_t found_size = 0;
		void *found_address = NULL;

		size_t min_found_enough_size = 0;
		void * min_found_enough_address = NULL;

		//search
		for (char *i = memory; i < memory+MEMORY_SIZE; i++) {
			size_t alloc_at_addr = getAllocSizeAtAddr(i);

			if (alloc_at_addr == 0 && found_address == NULL) {
				found_address = i;
			}

			if (alloc_at_addr == 0 && i != memory+MEMORY_SIZE-1) {
				found_size++;
			} else {
				if (found_size >= requested_size
				&&	(found_size < min_found_enough_size || min_found_enough_size == 0)
				) {
					min_found_enough_address = found_address;
					found_address = NULL;

					min_found_enough_size = found_size;
					found_size = 0;

				}

				i += alloc_at_addr;
			}
		}

		return min_found_enough_address;
	}

	void registerAllocation(void *addr, size_t size) {
		for (int i = 0; i < MAX_ALLOC_COUNT; i++) {
			if (alloc_address[i] == NULL) {
				//free alloc meta found, register
				alloc_address[i] = addr;
				alloc_size[i] = size;
				break;
			}
		}
	}

	void unregisterAllocation(void *addr) {
		for (int i = 0; i < MAX_ALLOC_COUNT; i++) {
			if (alloc_address[i] == addr) {
				//needed alloc meta, free
				alloc_address[i] = NULL;
				alloc_size[i] = 0;
				break;
			}
		}
	}

public:
	static MemoryContainer& get() {
		return instance;
	}

	~MemoryContainer() {
		delete[] memory;
	}

	void *allocate(size_t requested_size) {

		current_alloc_count++;
		if (current_alloc_count > MAX_ALLOC_COUNT) {
			throw "fail";	/**< @todo be more specific */
		}

		//get free
		void *found_address = NULL;
		found_address = getFreeAddress(requested_size);

		if (found_address == NULL) {
			throw "out of memory"; /**< @todo be more precise */
		}

		//allocate
		registerAllocation(found_address, requested_size);

		return found_address;
	}

	void free(void* m) {
		unregisterAllocation(m);
		current_alloc_count--;
	}
};

MemoryContainer MemoryContainer::instance = MemoryContainer();

#endif /* MEMORYCONTAINER_HPP_ */

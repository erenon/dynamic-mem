/*
 * MemoryContainer.hpp
 *
 *  Created on: 2011.04.10.
 *      Author: erenon
 */

#ifndef MEMORYCONTAINER_HPP_
#define MEMORYCONTAINER_HPP_

#include <cstddef>

/**
 * Size of memory to allocate on consturct
 */
#define MEMORY_SIZE (1 << 10)

/**
 * Maximum count of allocations at the same time
 */
#define MAX_ALLOC_COUNT 20

/**
 * Manages dynamically allocated memory.
 *
 * Allocates dynamic memory to other classes
 * using it's own, previously allocated memory.
 */

class MemoryContainer {
	/**
	 * Singleton instance
	 */
	static MemoryContainer instance;

	/**
	 * Allocated private memory
	 */
	char *memory;

	/**
	 * Count of currently allocated memory fields
	 */
	int current_alloc_count;

	/**
	 * Memory meta information. Holds the starting
	 * addresses of the allocations.
	 *
	 * @see MemoryContainer::memory
	 */
	void* alloc_address[MAX_ALLOC_COUNT];

	/**
	 * Memory meta information. Holds the size of
	 * the current allocations.
	 *
	 * @see MemoryContainer::memory
	 */
	size_t alloc_size[MAX_ALLOC_COUNT];

	/**
	 * Allocates private memory, resets
	 * meta informations, sets the
	 * singleton instance.
	 */
	MemoryContainer() {
		memory = new char[MEMORY_SIZE];
		current_alloc_count = 0;

		for (int i = 0; i < MAX_ALLOC_COUNT; i++) {
			alloc_address[i] = NULL;
			alloc_size[i] = 0;
		}

		instance = *this;
	}

	/**
	 * Gets the size of an allocation on
	 * the given address.
	 *
	 * @param addr Address of the allocation
	 * @returns Size of the allocation, 0 if no allocation
	 * on the given address found.
	 */
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

	/**
	 * Gets a free block in the memory.
	 *
	 * Uses best fit algorithm.
	 *
	 * @param requested_size The size of the requested block in bytes
	 * @returns Address of the block found
	 */
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

	/**
	 * Updates meta information on memory allocation
	 * based on the given values.
	 * Puts the meta info in the first unused block.
	 *
	 * @param addr Address of the allocation
	 * @param size Size of the allocation
	 */
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

	/**
	 * Updates meta information on memory free
	 * based on the given address. Deletes the
	 * related entries.
	 *
	 * @param addr Address of the freed block
	 */
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
	/**
	 * Gets the singleton instance
	 *
	 * @returns The singleton instance
	 */
	static MemoryContainer& get() {
		return instance;
	}

	/**
	 * Deletes private memory
	 */
	~MemoryContainer() {
		delete[] memory;
	}

	/**
	 * Allocates a memory block in the private dynamic memory.
	 *
	 * @param requested_size Size of the memory to allocate
	 * @returns Address of the allocated memory
	 */
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

	/**
	 * Frees the memory block on the given address.
	 *
	 * @param m Starting address of the memory block to free.
	 */
	void free(void* m) {
		unregisterAllocation(m);
		current_alloc_count--;
	}
};

/**
 * Creates the singleton instance.
 */
MemoryContainer MemoryContainer::instance = MemoryContainer();

#endif /* MEMORYCONTAINER_HPP_ */

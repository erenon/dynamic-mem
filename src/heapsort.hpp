#ifndef HEAPSORT_HPP_
#define HEAPSORT_HPP_

#include "Memory.hpp"

/**
 * Swaps two items of and array.
 *
 * @param array Array
 * @param a index of the item to swap
 * @param b index of the other item to swap
 */
template<typename T>
void swapItem(Memory<T> *array[], int a, int b) {
	Memory<T> *temp;

	temp = array[a];
	array[a] = array[b];
	array[b] = temp;
}

/**
 * Shifts the given element to the lowest
 * available poistion.
 *
 * @param a Array
 * @param root the starting element to shift down
 * @param end last index to care about
 */
template<typename T>
void shiftDown(Memory<T> *a[], int root, int end) {
	while (root *2 + 1 <= end) {
		int child = root *2 + 1;
		int swap = root;

		if (*a[swap] < *a[child]) {
			swap = child;
		}
		if (child < end && *a[swap] < *a[child+1]) {
			swap = child+1;
		}
		if (swap != root) {
			swapItem(a, root, swap);

			root = swap;
		} else {
			break;
		}
	}
}

/**
 * Creates the "heap property" using the given array
 *
 * @param a Used array
 * @param len Length of the array
 */
template<typename T>
void heapify(Memory<T> *a[], int len) {
	int start = len/2 - 1;

	while (start >= 0) {
		shiftDown(a, start, len -1);
		start--;
	}
}

/**
 * Sorts the elements of the given array.
 *
 * Uses heapsort algorithm. Sort is done in place.
 *
 * @param a Array to sort
 * @param len Length of the array
 */
template<typename T>
void heapsort(Memory<T> *a[], int len) {
	heapify(a, len);

	int end = len -1;

	while (end > 0) {
		swapItem(a, end, 0);
		shiftDown(a, 0, end -1);
		end--;
	}
}

#endif /* HEAPSORT_HPP_ */

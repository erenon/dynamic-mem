#ifndef HEAPSORT_HPP_
#define HEAPSORT_HPP_

#include "Memory.hpp"

template<typename T>
void swapItem(Memory<T> **array, int a, int b) {
	Memory<T> *temp;

	temp = array[a];
	array[a] = array[b];
	array[b] = temp;
}

template<typename T>
void shiftDown(Memory<T> *a[], int start, int end) {
	int root = start;

	while (root *2 + 1 <= end) {
		int child = root *2 + 1;
		int swap = root;

		if (*a[swap] < *a[child]) {
			swap = child;
		}
		if (child < end && *a[swap] < *a[child+1]) {
			swap = child+1;
		}
		if (swap  != root) {
			swapItem(a, root, swap);

			root = swap;
		} else {
			break;
		}
	}
}

template<typename T>
void heapify(Memory<T> *a[], int len) {
	int start = len/2 - 1;

	while (start >= 0) {
		shiftDown(a, start, len -1);
		start--;
	}
}

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

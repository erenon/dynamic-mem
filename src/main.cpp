#include <iostream>
#include <cstddef>
#include "Memory.hpp"
#include "heapsort.hpp"

#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

using namespace std;

int main() {
	//init
	srand(time(0));

	int size = 20;
	Memory<int> *array[size];

	cout << "Custom dynamic memory, heapsort test" << endl;
	cout << "Current items:" << endl;

	try {
		for (int i = 0; i < size; i++) {
			array[i] = new Memory<int>;
			*(array[i]) = rand() % 50;
			cout << *(array[i]) << "\t";
		}
	} catch (const char* msg) {
		cout << msg;
		cout << endl;
		return 1;
	}

	cout << endl << "After heapsort" << endl;

	heapsort(array, size);

#ifdef DEBUG
	MemoryContainer::get().dumpMemory();
#endif

	for (int i = 0; i < size; i++) {
		cout << *(array[i]) << "\t";
		delete array[i];
	}

	cout << endl;
}

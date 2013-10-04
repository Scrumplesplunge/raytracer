#pragma once
#include <cstdlib>

template <typename T> class Array {
	/*
		Array Class.

		Datatype invariant: 0 <= size <= space, data points to an array of
		'space' containers with 'size' containers occupied by elements,
		or data = NULL <--> space = 0.
	*/
	private:
		unsigned int size, space;
		T** data;

		void startup(const Array&);
		void cleanup();
		void increase(unsigned int);
		void decrease(unsigned int);
	public:
		Array();
		Array(const Array&);
		~Array();

		void operator=(const Array&);
		T& operator[](unsigned int) const;
		
		void push(const T&);
		T pop();

		void unshift(const T&);
		T shift();

		void insert(unsigned int, const T&);
		T remove(unsigned int);

		// High level stuff.
		Array slice(unsigned int) const;
		Array slice(unsigned int, unsigned int) const;
		Array concat(const Array&) const;
		Array merge(const Array&, int (*)(T, T)) const;

		Array sort(int (*)(T, T)) const;

		unsigned int length() const;
};

template <typename T> void Array<T>::startup(const Array<T>& parent) {
	size = parent.size;
	space = parent.space;
	
	// Allocate some memory!
	data = (T**) malloc(space * sizeof(T*));
	
	// Copy across all the stored values from the other array.
	for (unsigned int i = 0; i < size; i++) data[i] = new T(parent[i]);
}

// Complexity : O(n)
template <typename T> void Array<T>::cleanup() {
	// data = NULL --> space = 0 --> size = 0, hence nothing to delete.
	if (data != NULL) {
		// Loop invariant: there are 'size' elements left to delete.
		while (size --> 0) {	// 'size' + 1 elements left to delete.
			delete data[size];	// 'size' elements left.
		}
		// Termination: size <= 0 --> size = 0 and nothing left to delete.
		// We can now gracefully free the containers.
		free(data);
		data = NULL;
		size = space = 0;
	}
}

template <typename T> void Array<T>::increase(unsigned int desiredSize) {
	// Don't do anything if we already have at least this size.
	if (desiredSize <= size) return;

	// Double the space or initialise to 1 if uninitialised.
	while (space < desiredSize) space = (space > 0) ? (space << 1) : 1;

	// Create the new array.
	T** tmp = (T**) malloc(space * sizeof(T*));

	// Copy across the stored values from the old array and then free it.
	if (data != NULL) {
		for (unsigned int i = 0; i < size; i++) tmp[i] = data[i];
		// Remove the old array.
		free(data);
	}

	// Fill in the other slots with fresh instances.
	for (unsigned int i = size; i < desiredSize; i++) tmp[i] = new T();

	// Replace with the new array.
	data = tmp;
}

template <typename T> void Array<T>::decrease(unsigned int desiredSize) {
	// Don't do anything if the desired size is larger than the actual size.
	if (size <= desiredSize) return;

	// Half the space.
	while ((space >> 1) >= desiredSize) space = (space >> 1);

	// Free all elements that we are discarding.
	for (unsigned int i = desiredSize; i < size; i++) delete data[i];

	// Create the new array (if space > 0)
	if (space > 0) {
		T** tmp = (T**) malloc(space * sizeof(T*));

		// Copy across old values.
		for (unsigned int i = 0; i < desiredSize; i++) tmp[i] = data[i];

		// Free the old array.
		free(data);
		data = tmp;
	} else if (data != NULL) {
		free(data);
		data = NULL;
	}
}

// Initialisation: size = space = 0, data = NULL --> DTI
template <typename T> Array<T>::Array() : size(0), space(0), data(NULL) {}

template <typename T> Array<T>::Array(const Array<T>& parent) {
	startup(parent);
}

template <typename T> Array<T>::~Array() {
	cleanup();
}

template <typename T> void Array<T>::operator=(const Array<T>& parent) {
	// Deal with the previous value.
	cleanup();

	// Move in the new values!
	startup(parent);
}

template <typename T> T& Array<T>::operator[](unsigned int index) const {
	// I'm too lazy to make it throw something sensible.
	if (index >= size) throw 1;
	return *data[index];
}

template <typename T> void Array<T>::push(const T& element) {
	// size is about to be incremented so in order to maintain
	// size <= space, space must increase.
	if (size == space) increase(size + 1);
	data[size++] = new T(element);
}

template <typename T> T Array<T>::pop() {
	// Can't pop on an empty array.
	if (size == 0) throw 1;

	// Fetch the last element.
	T* ptr = data[--size];

	// If we are using less than a quarter of the available space, half the buffer size.
	if ((size << 2) <= space) decrease(size << 1);
	
	// Output the last element.
	T out = *ptr;
	delete ptr;
	return out;
}

template <typename T> void Array<T>::unshift(const T& element) {
	if (size == space) increase(size + 1);

	// Increase the size.
	size++;

	// Shuffle everything up.
	for (unsigned int i = size; i > 0; i--) data[i] = data[i - 1];
	data[0] = new T(element);
}

template <typename T> T Array<T>::shift() {
	// Can't shift on an empty array.
	if (size == 0) throw 1;

	// Fetch the first element.
	T* ptr = data[0];

	// Decrease the size value.
	size--;

	// Shuffle everything down.
	for (unsigned int i = 0; i < size; i++) data[i] = data[i + 1];

	// If we are using less than a quarter of the available space, half the buffer size.
	if ((size << 2) <= space) decrease(size << 1);

	// Output the element.
	T out = *ptr;
	delete ptr;
	return out;
}

template <typename T> void Array<T>::insert(unsigned int index, const T& element) {
	// Can't insert before the beginning or after the end.
	if (index < 0 || size < index) throw 1;

	if (size == space) increase(size + 1);
	
	// Increase the size.
	size++;
	
	// Shuffle everything up.
	for (unsigned int i = size; i > index; i--) data[i] = data[i - 1];
	data[index] = new T(element);
}

template <typename T> T Array<T>::remove(unsigned int index) {
	// Can't remove if index is out of bounds.
	if (index < 0 || size <= index) throw 1;

	// Fetch the element.
	T* ptr = data[index];

	// Decrease the size value.
	size--;

	// Shuffle everything down.
	for (unsigned int i = index; i < size; i++) data[i] = data[i + 1];

	// If we are using less than a quarter of the available space, half the buffer size.
	if ((size << 2) <= space) decrease(size << 1);

	// Output the element.
	T out = *ptr;
	delete ptr;
	return out;
}

template <typename T> Array<T> Array<T>::slice(unsigned int index) const {
	// Output array.
	Array<T> out;

	// Add every element after and including the one at index.
	for (unsigned int i = index; i < size; i++) {
		out.push(operator[](i));
	}
	return out;
}

template <typename T> Array<T> Array<T>::slice(unsigned int index, unsigned int len) const {
	// Output array.
	Array<T> out;

	// Add up to length elements after and including index.
	unsigned int n = index + len;
	n = n < size ? n : size;
	for (unsigned int i = 0; i < n; i++) {
		out.push(data[i]);
	}
	return out;
}

template <typename T> Array<T> Array<T>::concat(const Array& array) const {
	// Output array.
	Array<T> out(*this);

	// Add all the elements from the second array.
	for (unsigned int i = 0; i < array.size; i++) {
		out.push(array[i]);
	}
	return out;
}

template <typename T> Array<T> Array<T>::merge(const Array& array, int (*compare)(T, T)) const {
	// Output array.
	Array<T> out;

	// Add all the elements.
	unsigned i = 0, j = 0;
	while (i < size && j < array.size) {
		int comp = compare(operator[](i), array[j]);
		if (comp <= 0) {
			out.push(operator[](i++));
		} else {
			out.push(array[j++]);
		}
	}

	// Add the remaining elements.
	while (i < size) out.push(operator[](i++));
	while (j < array.size) out.push(array[j++]);
	return out;
}

template <typename T> Array<T> Array<T>::sort(int (*compare)(T, T)) const {
	// Output array (A clone of this).
	Array<T> out;

	// Insertion sort.
	for (unsigned int i = 0; i < size; i++) {
		unsigned int j = 0;
		while (j < out.length() && compare(out[j], operator[](i)) <= 0) j++;
		out.insert(j, operator[](i));
	}

	// Return the sorted array.
	return out;
}

template <typename T> unsigned int Array<T>::length() const {
	return size;
}


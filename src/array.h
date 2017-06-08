#pragma once
#include <cstdlib>
#include <vector>

template <typename T>
class Array {
  /*
          Array Class.

          Datatype invariant: 0 <= size <= space, data points to an array of
          'space' containers with 'size' containers occupied by elements,
          or data = NULL <--> space = 0.
  */
 private:
  std::vector<T> container;

 public:
  Array();
  Array(const Array&);
  ~Array();

  void operator=(const Array&);
  const T& operator[](unsigned int) const;
  T& operator[](unsigned int);

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

// Initialisation: size = space = 0, data = NULL --> DTI
template <typename T>
Array<T>::Array() {
  container.reserve(16);
}

template <typename T>
Array<T>::Array(const Array<T>& parent) {
  container = parent.container;
}

template <typename T>
Array<T>::~Array() {}

template <typename T>
void Array<T>::operator=(const Array<T>& parent) {
  container = parent.container;
}

template <typename T>
const T& Array<T>::operator[](unsigned int index) const {
  return container.at(index);
}

template <typename T>
T& Array<T>::operator[](unsigned int index) {
  return container.at(index);
}

template <typename T>
void Array<T>::push(const T& element) {
  container.push_back(element);
}

template <typename T>
T Array<T>::pop() {
  return container.pop_back();
}

template <typename T>
void Array<T>::unshift(const T& element) {
  container.insert(container.begin(), element);
}

template <typename T>
T Array<T>::shift() {
  T out(container.at(0));
  container.erase(container.begin());
  return out;
}

template <typename T>
void Array<T>::insert(unsigned int index, const T& element) {
  container.insert(container.begin(), element);
}

template <typename T>
T Array<T>::remove(unsigned int index) {
  T out(container.at(index));
  container.erase(container.begin() + index);
  return out;
}

template <typename T>
Array<T> Array<T>::slice(unsigned int index) const {
  Array<T> out;
  out.container.assign(container.begin() + index, container.end());
  return out;
}

template <typename T>
Array<T> Array<T>::slice(unsigned int index, unsigned int len) const {
  Array<T> out;
  out.container.assign(container.begin() + index,
                       container.begin() + index + len);
  return out;
}

template <typename T>
Array<T> Array<T>::concat(const Array& array) const {
  Array<T> out(*this);
  out.container.insert(out.container.end(), array.container.begin(),
                       array.container.end());
  return out;
}

template <typename T>
Array<T> Array<T>::merge(const Array& array, int (*compare)(T, T)) const {
  // Output array.
  Array<T> out;
  out.container.reserve(container.size() + array.container.size());

  // Add all the elements.
  unsigned i = 0, m = container.size(), j = 0, n = array.container.size();
  while (i < m && j < n) {
    int comp = compare(operator[](i), array[j]);
    if (comp <= 0) {
      out.container.push_back(container[i++]);
    } else {
      out.container.push_back(array.container[j++]);
    }
  }

  // Add the remaining elements.
  while (i < m) out.container.push_back(container[i++]);
  while (j < n) out.container.push_back(array.container[j++]);
  return out;
}

template <typename T>
Array<T> Array<T>::sort(int (*compare)(T, T)) const {
  // Output array (A clone of this).
  Array<T> out;
  out.container.reserve(container.size());

  // Insertion sort.
  for (unsigned int i = 0, size = container.size(); i < size; i++) {
    unsigned int j = 0, n = out.container.size();
    while (j < n && compare(out.container[j], container[i]) <= 0) j++;
    out.container.insert(out.container.begin() + j, container[i]);
  }

  // Return the sorted array.
  return out;
}

template <typename T>
unsigned int Array<T>::length() const {
  return container.size();
}

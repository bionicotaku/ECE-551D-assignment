#include "IntArray.h"

#include <assert.h>

#include <ostream>

IntArray::IntArray() : data(NULL), numElements(0) {
}
IntArray::IntArray(int n) : numElements(n) {
  data = new int[numElements];
}

IntArray::IntArray(const IntArray & rhs) : data(NULL) {
  *this = rhs;
}

IntArray::~IntArray() {
  delete[] data;
}

IntArray & IntArray::operator=(const IntArray & rhs) {
  if (this == &rhs) {
    return *this;
  }
  delete[] data;
  numElements = rhs.numElements;
  data = new int[numElements];
  std::copy(rhs.data, rhs.data + rhs.numElements, data);
  return *this;
}

const int & IntArray::operator[](int index) const {
  assert(index >= 0 and index < this->numElements);
  return data[index];
}
int & IntArray::operator[](int index) {
  assert(index >= 0 and index < this->numElements);
  return data[index];
}

int IntArray::size() const {
  return numElements;
}

bool IntArray::operator==(const IntArray & rhs) const {
  if (rhs.numElements != numElements)
    return false;
  for (int i = 0; i < numElements; i++) {
    if (data[i] != rhs.data[i])
      return false;
  }
  return true;
}

bool IntArray::operator!=(const IntArray & rhs) const {
  return !(*this == rhs);
}

std::ostream & operator<<(std::ostream & s, const IntArray & rhs) {
  s << "{";
  for (int i = 0; i < rhs.size(); ++i) {
    s << rhs[i];
    if (i < rhs.size() - 1) {
      s << ", ";
    }
  }
  s << "}";
  return s;
}

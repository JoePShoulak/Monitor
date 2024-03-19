#include "MenuItem.h"

template <typename T, int N>
MenuItem<T,N>::MenuItem(const CircularBuffer<T, N>& buffer) {
  _buffer = buffer;
}

template <typename T, int N>
T MenuItem<T,N>::current() {
  return _buffer.current();  
}

template <typename T, int N>
void MenuItem<T,N>::next() {
  _buffer.next();
}
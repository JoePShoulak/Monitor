#ifndef MenuItem_h
#define MenuItem_h

#include <Arduino.h>
#include "CircularBuffer.h"

template <typename T, int N>
class MenuItem
{
public:
  MenuItem(const CircularBuffer<T, N>& buffer);
  
  T current();
  void next();

private:
  const CircularBuffer<T, N>& _buffer;
};

#endif

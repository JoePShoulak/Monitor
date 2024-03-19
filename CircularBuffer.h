#ifndef CircularBuffer_h
#define CircularBuffer_h

/**
 * A circular buffer of fixed size.
 * Appending can be done indefinitely,
 * as the index will just loop back around to the beginning.
 */
template<typename TYPE, int LEN>
class CircularBuffer {
  TYPE data[LEN];   //The data to store
  int counter = 0;  //The current index. This will always be between 0 and LEN.
  int total = 0;    //The total number of items in the buffer. This will never exceed LEN.

public:
  const int length = LEN;

  //A custom iterator for this class, to allow C++'s range based for loop syntax
  class iterator {
    const TYPE* ptr;
    int offset;

  public:
    iterator(const TYPE* buffer, int index)
      : ptr(buffer), offset(index) {}

    iterator operator++() {
      ++offset;
      return *this;
    }

    bool operator!=(const iterator& other) const {
      return offset != other.offset;
    }

    const TYPE& operator*() const {
      return ptr[offset % LEN];
    }
  };

  //Default constructor, don't initialize data.
  CircularBuffer() {}

  //Initialize the buffer with the contents of an array.
  //This allows for brace-enclosed initialization e.g.
  //CircularBuffer<int, 3> buffer = {1, 2, 3};
  template<typename... ARGS>
  CircularBuffer(const ARGS&... args) {
    this->populate(args...);
  }

  //Initialize data to a specific value, e.g. 0
  CircularBuffer(TYPE default_value) {
    for (int i = 0; i < LEN; ++i) {
      data[i] = default_value;
    }
  }

  //Get an element from the buffer. This is used when we're not modifying it.
  TYPE operator[](const int index) const {
    return data[index % LEN];
  }

  //Get a reference to an element from the buffer. This is used when we ARE modifying it.
  //The compiler knows the difference and will use this function only when needed.
  TYPE& operator[](const int index) {
    return data[index % LEN];
  }

  //Add a value to the buffer, and increment the current index
  void append(const TYPE value) {
    data[counter] = value;
    counter = (counter + 1) % LEN;
    if (total < LEN) ++total;
  }

  //Get the current index. Will always be a value from 0 to the length of the buffer
  int index() const {
    return counter;
  }

  //Get a reference to the item at the current index
  TYPE& current() {
    return data[counter];
  }

  //Get the total number of items in the buffer
  int count() const {
    return total;
  }

  //Move to the previous spot in the buffer
  void prev() {
    counter = (counter + LEN - 1) % LEN;
  }

  //Move to the next spot in the buffer
  void next() {
    counter = (counter + 1) % LEN;
  }

  //Helper method for C++ range based for loops
  iterator begin() const {
    return iterator(data, counter + LEN + LEN - total);
  }

  //Helper method for C++ range based for loops
  iterator end() const {
    return iterator(data, counter + LEN + LEN);
  }

  //Fill the buffer with the contents of an array.
  //This allows for brace-enclosed initialization e.g.
  //CircularBuffer<int, 3> buffer = {1, 2, 3};
  template<typename... ARGS>
  void populate(const TYPE& first, const ARGS&... args) {
    append(first);
    populate(args...);
  }

  void populate(const TYPE& arg) {
    append(arg);
  }
};

#endif

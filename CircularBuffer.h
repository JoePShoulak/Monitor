/**
 * A circular buffer of fixed size.
 * Appending can be done indefinitely,
 * as the index will just loop back around to the beginning.
 */
template<typename TYPE, int LEN>
class CircularBuffer {
  TYPE data[LEN];
  int counter = 0;

public:
  const int length = LEN;

  //A custom iterator for this class, to allow C++'s range based for loop syntax
  class iterator {
    const TYPE* ptr;
    int offset;

  public:
    iterator(const TYPE* buffer, int index) : ptr(buffer), offset(index) {}

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

  //Get a reference to an element from the buffer. This is used when we ARE not modifying it.
  //The compiler knows the difference and will use this function only when needed.
  TYPE& operator[](const int index) {
    return data[index % LEN];
  }

  //Add a value to the buffer, and increment the current index
  void append(const TYPE value) {
    data[counter] = value;
    counter = (counter + 1) % LEN;
  }

  //Get the current index. Will always be a value from 0 to the length of the buffer
  int index() const {
    return counter;
  }

  void up() {
    counter = (counter + LEN - 1) % LEN;
  }

  void down() {
    counter = (counter + 1) % LEN;
  }

  //Helper method for C++ range based for loops
  iterator begin() const {
    return iterator(data, counter + LEN);
  }

  //Helper method for C++ range based for loops
  iterator end() const {
    return iterator(data, counter + LEN + LEN);
  }
};

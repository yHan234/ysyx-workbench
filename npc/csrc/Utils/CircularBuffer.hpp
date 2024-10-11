#pragma once

#include "common.hpp"
#include <array>

template <class T, size_t Size>
class WriteOnlyCircularBuffer {
public:
  WriteOnlyCircularBuffer()
      : p(0) {}

  void Write(const T &e) {
    data[p++ % Size] = e;
    if (p == Size * 2) {
      p -= Size;
    }
  }

  void Write(T &&e) {
    data[p++ % Size] = std::move(e);
    if (p == Size * 2) {
      p -= Size;
    }
  }

  bool IsEmpty() {
    return p == 0;
  }

  class Iterator {
  public:
    Iterator(WriteOnlyCircularBuffer &buf, std::size_t read_p)
        : buf(buf), read_p(read_p) {}

    T &operator*() {
      return buf.data[read_p % Size];
    }

    Iterator &operator++() {
      read_p += 1;
      if (read_p == Size * 2) {
        read_p -= Size;
      }
      return *this;
    }

    bool operator==(const Iterator &other) const {
      return &buf == &other.buf && read_p == other.read_p;
    }

    bool operator!=(const Iterator &other) const {
      return !(*this == other);
    }

  private:
    WriteOnlyCircularBuffer &buf;
    size_t read_p;
  };

  Iterator begin() {
    return Iterator(*this, p < Size ? 0 : p - Size);
  }

  Iterator end() {
    return Iterator(*this, p);
  }

private:
  std::array<T, Size> data;
  size_t p;
};

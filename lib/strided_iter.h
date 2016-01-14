#ifndef SIMULEIOS_STRIDED_ITER_H
#define SIMULEIOS_STRIDED_ITER_H

#include <cassert>
#include <cstddef>
#include <iterator>

template <typename T>
class strided_iter {
 public:
  strided_iter() : data_(nullptr), stride_(0) {}
  strided_iter(T* data, size_t current, ptrdiff_t stride)
      : data_(data), current_(current), stride_(stride) {}

  T& operator*() { return data_[current_]; }
  const T& operator*() const { return data_[current_]; }

  T* operator->() { return data_ + current_; }
  const T* operator->() const { return data_ + current_; }

  T& operator[](size_t n) { return data_[current_ + n]; }
  const T& operator[](size_t n) const { return data_[current_ + n]; }

  strided_iter& operator++() {
    current_ += stride_;
    return *this;
  }

  strided_iter operator++(int) const {
    strided_iter ret(*this);
    ++ret;
    return ret;
  }

  strided_iter& operator--() {
    current_ -= stride_;
    return *this;
  }

  strided_iter operator--(int) const {
    strided_iter ret(*this);
    --ret;
    return ret;
  }

  strided_iter& operator+=(ptrdiff_t n) {
    current_ += n * stride_;
    return *this;
  }

  strided_iter& operator-=(ptrdiff_t n) {
    current_ -= n * stride_;
    return *this;
  }

  friend strided_iter operator+(strided_iter lhs, ptrdiff_t rhs) {
    lhs += rhs;
    return lhs;
  }

  friend ptrdiff_t operator-(const strided_iter& lhs, const strided_iter& rhs) {
    assert(lhs.stride_ == rhs.stride_);
    return (lhs.current_ - rhs.current_) / lhs.stride_;
  }

  friend strided_iter operator-(strided_iter lhs, ptrdiff_t n) {
    lhs -= n;
    return lhs;
  }

  friend bool operator==(const strided_iter& lhs, const strided_iter& rhs) {
    return lhs.current_ == rhs.current_ && lhs.data_ == rhs.data_ &&
           lhs.stride_ == rhs.stride_;
  }

  friend bool operator!=(const strided_iter& lhs, const strided_iter& rhs) {
    return !(lhs == rhs);
  }

  friend bool operator<(const strided_iter& lhs, const strided_iter& rhs) {
    assert(lhs.data_ == rhs.data_ && lhs.stride_ == rhs.stride_);
    return lhs.current_ < rhs.current_;
  }

  friend bool operator>(const strided_iter& lhs, const strided_iter& rhs) {
    return (rhs < lhs);
  }

  friend bool operator<=(const strided_iter& lhs, const strided_iter& rhs) {
    return !(rhs < lhs);
  }

  friend bool operator>=(const strided_iter& lhs, const strided_iter& rhs) {
    return !(lhs < rhs);
  }

  friend void swap(strided_iter& lhs, strided_iter& rhs) {
    using std::swap;
    swap(lhs.data_, rhs.data_);
    swap(lhs.current_, rhs.current_);
    swap(lhs.stride_, rhs.stride_);
  }

 private:
  T* data_;
  size_t current_;
  ptrdiff_t stride_;
};

template <typename T>
class strided_iter<const T> {
 public:
  strided_iter() : data_(nullptr), stride_(0) {}
  strided_iter(const T* data, size_t current, ptrdiff_t stride)
      : data_(data), current_(current), stride_(stride) {}

  const T& operator*() const { return data_[current_]; }

  const T* operator->() const { return data_ + current_; }

  const T& operator[](size_t n) const { return data_[current_ + n]; }

  strided_iter& operator++() {
    current_ += stride_;
    return *this;
  }

  strided_iter operator++(int) const {
    strided_iter ret(*this);
    ++ret;
    return ret;
  }

  strided_iter& operator--() {
    current_ -= stride_;
    return *this;
  }

  strided_iter operator--(int) const {
    strided_iter ret(*this);
    --ret;
    return ret;
  }

  strided_iter& operator+=(ptrdiff_t n) {
    current_ += n * stride_;
    return *this;
  }

  strided_iter& operator-=(ptrdiff_t n) {
    current_ -= n * stride_;
    return *this;
  }

  friend strided_iter operator+(strided_iter lhs, ptrdiff_t rhs) {
    lhs += rhs;
    return lhs;
  }

  friend ptrdiff_t operator-(const strided_iter& lhs, const strided_iter& rhs) {
    assert(lhs.stride_ == rhs.stride_);
    return (lhs.current_ - rhs.current_) / lhs.stride_;
  }

  friend strided_iter operator-(strided_iter lhs, ptrdiff_t n) {
    lhs -= n;
    return lhs;
  }

  friend bool operator==(const strided_iter& lhs, const strided_iter& rhs) {
    return lhs.current_ == rhs.current_ && lhs.data_ == rhs.data_ &&
           lhs.stride_ == rhs.stride_;
  }

  friend bool operator!=(const strided_iter& lhs, const strided_iter& rhs) {
    return !(lhs == rhs);
  }

  friend bool operator<(const strided_iter& lhs, const strided_iter& rhs) {
    assert(lhs.data_ == rhs.data_ && lhs.stride_ == rhs.stride_);
    return lhs.current_ < rhs.current_;
  }

  friend bool operator>(const strided_iter& lhs, const strided_iter& rhs) {
    return (rhs < lhs);
  }

  friend bool operator<=(const strided_iter& lhs, const strided_iter& rhs) {
    return !(rhs < lhs);
  }

  friend bool operator>=(const strided_iter& lhs, const strided_iter& rhs) {
    return !(lhs < rhs);
  }

  friend void swap(strided_iter& lhs, strided_iter& rhs) {
    using std::swap;
    swap(lhs.data_, rhs.data_);
    swap(lhs.current_, rhs.current_);
    swap(lhs.stride_, rhs.stride_);
  }

 private:
  const T* data_;
  size_t current_;
  ptrdiff_t stride_;
};

template <typename T>
strided_iter<T> make_strided_iter(T* data, size_t current, ptrdiff_t stride) {
  return strided_iter<T>(data, current, stride);
}

template <typename T>
struct std::iterator_traits<strided_iter<T>> {
  using value_type = T;
  using difference_type = ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;
};

template <typename T>
struct std::iterator_traits<strided_iter<const T>> {
  using value_type = T;
  using difference_type = ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;
};

#endif

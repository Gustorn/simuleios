#ifndef SIMULEIOS_DIRECTIONAL_ITER_H
#define SIMULEIOS_DIRECTIONAL_ITER_H

#include <cassert>
#include <cstddef>
#include <iterator>
#include <utility>

#include "utility.h"

template <typename T, template <typename> class I>
class directional_iter {
 public:
  directional_iter() : parent_(nullptr), current_(0) {}
  directional_iter(T& parent, size_t current)
      : parent_(&parent), current_(current) {}

  I<T> operator*() { return I<T>(*parent_, current_); }

  I<const T> operator*() const { return I<const T>(*parent_, current_); }

  I<T>* operator->() {
    I<T>* typed_temp = reinterpret_cast<I<T>*>(temporary_);
    *typed_temp = **this;
    return typed_temp;
  }

  const I<const T>* operator->() const {
    I<const T>* typed_temp = reinterpret_cast<I<const T>*>(temporary_);
    *typed_temp = **this;
    return typed_temp;
  }

  I<T> operator[](size_t index) { return I<T>(*parent_, current_ + index); }

  I<const T> operator[](size_t index) const {
    return I<const T>(*parent_, current_ + index);
  }

  directional_iter& operator++() {
    ++current_;
    return *this;
  }

  directional_iter operator++(int) const {
    directional_iter ret(*this);
    ++ret;
    return ret;
  }

  directional_iter& operator--() {
    --current_;
    return *this;
  }

  directional_iter operator--(int) const { return *this; }

  directional_iter& operator+=(ptrdiff_t n) {
    current_ += n;
    return *this;
  }

  directional_iter& operator-=(ptrdiff_t n) {
    current_ -= n;
    return *this;
  }

  friend directional_iter operator+(directional_iter lhs, ptrdiff_t n) {
    lhs += n;
    return lhs;
  }

  friend ptrdiff_t operator-(const directional_iter& lhs,
                             const directional_iter& rhs) {
    return lhs.current_ - rhs.current_;
  }

  friend directional_iter operator-(directional_iter lhs, ptrdiff_t n) {
    lhs += n;
    return lhs;
  }

  friend bool operator==(const directional_iter& lhs,
                         const directional_iter& rhs) {
    return lhs.current_ == rhs.current_ && lhs.parent_ == rhs.parent_;
  }

  friend bool operator!=(const directional_iter& lhs,
                         const directional_iter& rhs) {
    return !(lhs == rhs);
  }

  friend bool operator<(const directional_iter& lhs,
                        const directional_iter& rhs) {
    assert(lhs.parent_ == rhs.parent_);
    return lhs.current_ < rhs.current_;
  }

  friend bool operator>(const directional_iter& lhs,
                        const directional_iter& rhs) {
    return rhs < lhs;
  }

  friend bool operator<=(const directional_iter& lhs,
                         const directional_iter& rhs) {
    return !(rhs < lhs);
  }

  friend bool operator>=(const directional_iter& lhs,
                         const directional_iter& rhs) {
    return !(lhs < rhs);
  }

  friend void swap(directional_iter& lhs, directional_iter& rhs) {
    using std::swap;
    swap(lhs.parent_, rhs.parent_);
    swap(lhs.current_, rhs.current_);
    swap(lhs.temporary_, rhs.temporary_);
  }

 private:
  T* parent_;
  size_t current_;
  mutable char temporary_[compile_max(sizeof(I<T>), sizeof(I<const T>))];
};

template <typename T, template <typename> class I>
class directional_iter<const T, I> {
 public:
  directional_iter() : parent_(nullptr), current_(0) {}
  directional_iter(const T& parent, size_t current)
      : parent_(&parent), current_(current) {}

  I<const T> operator*() const { return I<const T>(*parent_, current_); }

  const I<const T>* operator->() const {
    I<const T>* typed_temp = reinterpret_cast<I<const T>*>(temporary_);
    *typed_temp = **this;
    return typed_temp;
  }

  I<const T> operator[](size_t index) const {
    return I<const T>(*parent_, current_ + index);
  }

  directional_iter& operator++() {
    ++current_;
    return *this;
  }

  directional_iter operator++(int) const {
    directional_iter ret(*this);
    ++ret;
    return ret;
  }

  directional_iter& operator--() {
    --current_;
    return *this;
  }

  directional_iter operator--(int) const { return *this; }

  directional_iter& operator+=(ptrdiff_t n) {
    current_ += n;
    return *this;
  }

  directional_iter& operator-=(ptrdiff_t n) {
    current_ -= n;
    return *this;
  }

  friend directional_iter operator+(directional_iter lhs, ptrdiff_t n) {
    lhs += n;
    return lhs;
  }

  friend ptrdiff_t operator-(const directional_iter& lhs,
                             const directional_iter& rhs) {
    return lhs.current_ - rhs.current_;
  }

  friend directional_iter operator-(directional_iter lhs, ptrdiff_t n) {
    lhs += n;
    return lhs;
  }

  friend bool operator==(const directional_iter& lhs,
                         const directional_iter& rhs) {
    return lhs.current_ == rhs.current_ && lhs.parent_ == rhs.parent_;
  }

  friend bool operator!=(const directional_iter& lhs,
                         const directional_iter& rhs) {
    return !(lhs == rhs);
  }

  friend bool operator<(const directional_iter& lhs,
                        const directional_iter& rhs) {
    assert(lhs.parent_ == rhs.parent_);
    return lhs.current_ < rhs.current_;
  }

  friend bool operator>(const directional_iter& lhs,
                        const directional_iter& rhs) {
    return rhs < lhs;
  }

  friend bool operator<=(const directional_iter& lhs,
                         const directional_iter& rhs) {
    return !(rhs < lhs);
  }

  friend bool operator>=(const directional_iter& lhs,
                         const directional_iter& rhs) {
    return !(lhs < rhs);
  }

  friend void swap(directional_iter& lhs, directional_iter& rhs) {
    using std::swap;
    swap(lhs.parent_, rhs.parent_);
    swap(lhs.current_, rhs.current_);
    swap(lhs.temporary_, rhs.temporary_);
  }

 private:
  const T* parent_;
  size_t current_;
  mutable char temporary_[compile_max(sizeof(I<T>), sizeof(I<const T>))];
};

template <typename T, template <typename> class I>
struct std::iterator_traits<directional_iter<T, I>> {
  using value_type = I<T>;
  using difference_type = ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;
};

template <typename T, template <typename> class I>
struct std::iterator_traits<directional_iter<const T, I>> {
  using value_type = I<const T>;
  using difference_type = ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;
};

template <template <typename> class I, typename T>
directional_iter<T, I> make_directional_iter(T& parent, size_t current) {
  return directional_iter<T, I>(parent, current);
}

#endif

#ifndef SIMULEIOS_ROW_H
#define SIMULEIOS_ROW_H

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

#include "directional_iter.h"
#include "strided_iter.h"
#include "utility.h"

template <typename T>
class row_view_t {
 public:
  using value_type = typename T::value_type;

 public:
  row_view_t() : parent_(nullptr), row_(0) {}
  row_view_t(T& parent, size_t row) : parent_(&parent), row_(row) {}

  value_type& operator[](size_t column) { return (*parent_)(row_, column); }
  const value_type& operator[](size_t column) const {
    return (*parent_)(row_, column);
  }

  size_t size() const { return parent_->columns(); }
  size_t index() const { return row_; }

  value_type* begin() { return parent_->data() + row_ * parent_->columns(); }
  const value_type* begin() const {
    return parent_->data() + row_ * parent_->columns();
  }
  const value_type* cbegin() const { return begin(); }

  value_type* end() { return begin() + parent_->columns(); }
  const value_type* end() const { return begin() + parent_->columns(); }
  const value_type* cend() const { return end(); }

 private:
  T* parent_;
  size_t row_;
};

template <typename T>
class row_view_t<const T> {
 public:
  using value_type = typename T::value_type;

 public:
  row_view_t() : parent_(nullptr), row_(0) {}
  row_view_t(const T& parent, size_t row) : parent_(&parent), row_(row) {}

  const value_type& operator[](size_t column) const {
    return (*parent_)(row_, column);
  }

  size_t size() const { return parent_->columns(); }
  size_t index() const { return row_; }

  const value_type* begin() const {
    return parent_->data() + row_ * parent_->columns();
  }
  const value_type* cbegin() const { return begin(); }

  const value_type* end() const { return begin() + parent_->columns(); }
  const value_type* cend() const { return end(); }

 private:
  const T* parent_;
  size_t row_;
};

template <typename T>
row_view_t<T> make_row_view(T& parent, size_t row) {
  return row_view_t<T>(parent, row);
}

template <typename T>
void swap(row_view_t<T>&& lhs, row_view_t<T>&& rhs) {
  assert(false);
  std::swap_ranges(std::begin(lhs), std::end(lhs), std::begin(rhs));
}

template <typename T, typename U,
          typename std::enable_if<
              std::is_same<typename std::remove_const<T>::type,
                           typename std::remove_const<U>::type>::value,
              int>::type = 0>
bool operator==(const row_view_t<T>& lhs, const row_view_t<U>& rhs) {
  auto size = lhs.size();
  if (size != rhs.size()) return false;
  return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs));
}

template <typename T, typename U,
          typename std::enable_if<
              std::is_same<typename std::remove_const<T>::type,
                           typename std::remove_const<U>::type>::value,
              int>::type = 0>
bool operator!=(const row_view_t<T>& lhs, const row_view_t<U>& rhs) {
  return !(lhs == rhs);
}

template <typename T>
class column_view_t {
 public:
  using value_type = typename T::value_type;

 public:
  column_view_t() : parent_(nullptr), column_(0) {}
  column_view_t(T& parent, size_t column) : parent_(&parent), column_(column) {}

  value_type& operator[](size_t row) { return (*parent_)(row, column_); }
  const value_type& operator[](size_t row) const {
    return (*parent_)(row, column_);
  }

  size_t size() const { return parent_->rows(); }
  size_t index() const { return column_; }

  strided_iter<value_type> begin() {
    return make_strided_iter(parent_->data(), column_, parent_->columns());
  }

  strided_iter<const value_type> begin() const {
    return make_strided_iter(parent_->data(), column_, parent_->columns());
  }

  strided_iter<const value_type> cbegin() const { return begin(); }

  strided_iter<value_type> end() {
    auto rows = parent_->rows();
    auto columns = parent_->columns();
    return make_strided_iter(parent_->data(), column_ + rows * columns,
                             columns);
  }

  strided_iter<const value_type> end() const {
    auto rows = parent_->rows();
    auto columns = parent_->columns();
    return make_strided_iter(parent_->data(), column_ + rows * columns,
                             columns);
  }

  strided_iter<const value_type> cend() const { return end(); }

 private:
  T* parent_;
  size_t column_;
};

template <typename T>
class column_view_t<const T> {
 public:
  using value_type = typename T::value_type;

 public:
  column_view_t() : parent_(nullptr), column_(0) {}
  column_view_t(const T& parent, size_t column)
      : parent_(&parent), column_(column) {}

  const value_type& operator[](size_t row) const {
    return (*parent_)(row, column_);
  }

  size_t size() const { return parent_->rows(); }
  size_t index() const { return column_; }

  strided_iter<const value_type> begin() const {
    return make_strided_iter(parent_->data(), column_, parent_->columns());
  }

  strided_iter<const value_type> cbegin() const { return begin(); }

  strided_iter<const value_type> end() const {
    auto rows = parent_->rows();
    auto columns = parent_->columns();
    return make_strided_iter(parent_->data(), column_ + rows * columns,
                             columns);
  }

  strided_iter<const value_type> cend() const { return end(); }

 private:
  const T* parent_;
  size_t column_;
};

template <typename T>
column_view_t<T> make_column_view(T& parent, size_t column) {
  return column_view_t<T>(parent, column);
}

template <typename T>
void swap(column_view_t<T>&& lhs, column_view_t<T>&& rhs) {
  std::swap_ranges(std::begin(lhs), std::end(lhs), std::begin(rhs));
}

template <typename T, typename U,
          typename std::enable_if<
              std::is_same<typename std::remove_const<T>::type,
                           typename std::remove_const<U>::type>::value,
              int>::type = 0>
bool operator==(const column_view_t<T>& lhs, const column_view_t<U>& rhs) {
  auto size = lhs.size();
  if (size != rhs.size()) return false;
  return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs));
}

template <typename T, typename U,
          typename std::enable_if<
              std::is_same<typename std::remove_const<T>::type,
                           typename std::remove_const<U>::type>::value,
              int>::type = 0>
bool operator!=(const column_view_t<T>& lhs, const column_view_t<U>& rhs) {
  return !(lhs == rhs);
}

template <typename T>
class rows_view_t {
 public:
  rows_view_t() : parent_(nullptr) {}
  rows_view_t(T& parent) : parent_(&parent) {}

  row_view_t<T> operator[](size_t row) { return make_row_view(*parent_, row); }

  row_view_t<const T> operator[](size_t row) const {
    return make_row_view(*parent_, row);
  }

  directional_iter<T, row_view_t> begin() {
    return make_directional_iter<row_view_t>(*parent_, 0);
  }

  directional_iter<const T, row_view_t> begin() const {
    return make_directional_iter<row_view_t>(*parent_, 0);
  }

  directional_iter<const T, row_view_t> cbegin() const { return begin(); }

  directional_iter<T, row_view_t> end() {
    return make_directional_iter<row_view_t>(*parent_, parent_->rows());
  }

  directional_iter<const T, row_view_t> end() const {
    return make_directional_iter<row_view_t>(*parent_, parent_->rows());
  }

  directional_iter<const T, row_view_t> cend() const { return end(); }

 private:
  T* parent_;
};

template <typename T>
class rows_view_t<const T> {
 public:
  rows_view_t() : parent_(nullptr) {}
  rows_view_t(const T& parent) : parent_(&parent) {}

  row_view_t<const T> operator[](size_t row) const {
    return make_row_view(*parent_, row);
  }

  directional_iter<const T, row_view_t> begin() const {
    return make_directional_iter<row_view_t>(*parent_, 0);
  }

  directional_iter<const T, row_view_t> cbegin() const { return begin(); }

  directional_iter<const T, row_view_t> end() const {
    return make_directional_iter<row_view_t>(*parent_, parent_->rows());
  }

  directional_iter<const T, row_view_t> cend() const { return end(); }

 private:
  const T* parent_;
};

template <typename T>
rows_view_t<T> make_rows_view(T& parent) {
  return rows_view_t<T>(parent);
}

template <typename T>
class columns_view_t {
 public:
  columns_view_t() : parent_(nullptr) {}
  columns_view_t(T& parent) : parent_(&parent) {}

  column_view_t<T> operator[](size_t column) {
    return make_column_view(*parent_, column);
  }

  column_view_t<const T> operator[](size_t column) const {
    return make_column_view(*parent_, column);
  }

  directional_iter<T, column_view_t> begin() {
    return make_directional_iter<column_view_t>(*parent_, 0);
  }

  directional_iter<const T, column_view_t> begin() const {
    return make_directional_iter<column_view_t>(*parent_, 0);
  }

  directional_iter<const T, column_view_t> cbegin() const { return begin(); }

  directional_iter<T, column_view_t> end() {
    return make_directional_iter<column_view_t>(*parent_, parent_->columns());
  }

  directional_iter<const T, column_view_t> end() const {
    return make_directional_iter<column_view_t>(*parent_, parent_->columns());
  }

  directional_iter<const T, column_view_t> cend() const { return end(); }

 private:
  T* parent_;
};

template <typename T>
class columns_view_t<const T> {
 public:
  columns_view_t() : parent_(nullptr) {}
  columns_view_t(const T& parent) : parent_(&parent) {}

  column_view_t<const T> operator[](size_t column) const {
    return make_column_view(*parent_, column);
  }

  directional_iter<const T, column_view_t> begin() const {
    return make_directional_iter<column_view_t>(*parent_, 0);
  }

  directional_iter<const T, column_view_t> cbegin() const { return begin(); }

  directional_iter<const T, column_view_t> end() const {
    return make_directional_iter<column_view_t>(*parent_, parent_->columns());
  }

  directional_iter<const T, column_view_t> cend() const { return end(); }

 private:
  const T* parent_;
};

template <typename T>
columns_view_t<T> make_columns_view(T& parent) {
  return columns_view_t<T>(parent);
}

#endif

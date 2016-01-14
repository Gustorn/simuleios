#ifndef SIMULEIOS_ARRAY2D_H
#define SIMULEIOS_ARRAY2D_H

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <memory>
#include <utility>

#include "view.h"

template <typename T>
class array2d {
 public:
  using value_type = T;

 public:
  array2d(size_t rows, size_t columns)
      : data_(new T[rows * columns]), rows_(rows), columns_(columns) {}

  array2d(std::initializer_list<std::initializer_list<T>> values) {
    assert(values.size() > 0);
    assert(std::begin(values)->size() > 0);

    rows_ = values.size();
    columns_ = std::begin(values)->size();
    data_ = std::unique_ptr<T[]>(new T[rows_ * columns_]);

    size_t index = 0;
    for (auto&& row : values) {
      assert(row.size() == columns_);
      for (auto&& cell : row) {
        data_[index++] = std::move(cell);
      }
    }
  }

  array2d(const array2d& a)
      : data_(new T[a.rows_ * a.columns_]),
        rows_(a.rows_),
        columns_(a.columns_) {
    const auto* src = a.data_.get();
    std::copy(src, src + rows_ * columns_, data_.get());
  }

  array2d(array2d&& a)
      : data_(std::move(a.data_)),
        rows_(std::move(a.rows_)),
        columns_(std::move(a.columns_)) {}

  array2d& operator=(array2d rhs) {
    swap(rhs);
    return *this;
  }

  array2d& operator=(array2d&& rhs) {
    data_ = std::move(rhs.data_);
    rows_ = std::move(rhs.rows_);
    columns_ = std::move(rhs.columns_);
  }

  T& operator()(size_t row, size_t column) {
    assert(row < rows_ && column < columns_);
    return data_[row * columns_ + column];
  }

  const T& operator()(size_t row, size_t column) const {
    assert(row < rows_ && column < columns_);
    return data_[row * columns_ + column];
  }

  T* data() { return data_.get(); }
  const T* data() const { return data_.get(); }

  T* begin() { return data(); }
  const T* begin() const { return data(); }
  const T* cbegin() const { return begin(); }

  T* end() { return data() + rows_ * columns_; }
  const T* end() const { return data() + rows_ * columns_; }
  const T* cend() const { return end(); }

  size_t rows() const { return rows_; }
  size_t columns() const { return columns_; }

  rows_view_t<array2d<T>> rows_view() { return make_rows_view(*this); }
  rows_view_t<const array2d<T>> rows_view() const {
    return make_rows_view(*this);
  }

  columns_view_t<array2d<T>> columns_view() { return make_columns_view(*this); }
  columns_view_t<const array2d<T>> columns_view() const {
    return make_columns_view(*this);
  }

  void swap(array2d& rhs) {
    using std::swap;
    swap(data_, rhs.data_);
    swap(rows_, rhs.rows_);
    swap(columns_, rhs.columns_);
  }

 private:
  std::unique_ptr<T[]> data_;
  size_t rows_;
  size_t columns_;
};

template <typename T>
bool operator==(const array2d<T>& lhs, const array2d<T>& rhs) {
  auto rows = lhs.rows();
  auto columns = lhs.columns();

  if (rows != rhs.rows() || columns != rhs.columns()) return false;

  const auto* lhs_data = lhs.data();
  const auto* rhs_data = rhs.data();

  for (size_t i = 0; i < rows * columns; ++i) {
    if (lhs_data[i] != rhs_data[i]) return false;
  }

  return true;
}

template <typename T>
bool operator!=(const array2d<T>& lhs, const array2d<T>& rhs) {
  return !(lhs == rhs);
}

template <typename T>
void swap(array2d<T>& lhs, array2d<T>& rhs) {
  lhs.swap(rhs);
}

#endif

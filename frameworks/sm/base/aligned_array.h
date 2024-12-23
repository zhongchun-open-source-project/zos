// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_ALIGNED_ARRAY_
#define SM_BASE_ALIGNED_ARRAY_

#include "sm/base/checks.h"
#include "sm/base/aligned_malloc.h"

namespace sm {

// Wrapper class for aligned arrays. Every row (and the first dimension) are
// aligned to the given byte alignment.
template<typename T> class AlignedArray {
 public:
  AlignedArray(size_t rows, size_t cols, size_t alignment)
      : rows_(rows),
        cols_(cols) {
    RTC_CHECK_GT(alignment, 0u);
    head_row_ = static_cast<T**>(AlignedMalloc(rows_ * sizeof(*head_row_),
                                               alignment));
    for (size_t i = 0; i < rows_; ++i) {
      head_row_[i] = static_cast<T*>(AlignedMalloc(cols_ * sizeof(**head_row_),
                                                   alignment));
    }
  }

  ~AlignedArray() {
    for (size_t i = 0; i < rows_; ++i) {
      AlignedFree(head_row_[i]);
    }
    AlignedFree(head_row_);
  }

  T* const* Array() {
    return head_row_;
  }

  const T* const* Array() const {
    return head_row_;
  }

  T* Row(size_t row) {
    RTC_CHECK_LE(row, rows_);
    return head_row_[row];
  }

  const T* Row(size_t row) const {
    RTC_CHECK_LE(row, rows_);
    return head_row_[row];
  }

  T& At(size_t row, size_t col) {
    RTC_CHECK_LE(col, cols_);
    return Row(row)[col];
  }

  const T& At(size_t row, size_t col) const {
    RTC_CHECK_LE(col, cols_);
    return Row(row)[col];
  }

  size_t rows() const {
    return rows_;
  }

  size_t cols() const {
    return cols_;
  }

 private:
  size_t rows_;
  size_t cols_;
  T** head_row_;
};

}  // namespace sm

#endif  // SM_BASE_ALIGNED_ARRAY_

// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/aligned_array.h"

#include <stdint.h>

#include "testing/gtest/include/gtest/gtest.h"

namespace {

bool IsAligned(const void* ptr, size_t alignment) {
  return reinterpret_cast<uintptr_t>(ptr) % alignment == 0;
}

}  // namespace

namespace sm {

TEST(AlignedArrayTest, CheckAlignment) {
  AlignedArray<bool> arr(10, 7, 128);
  ASSERT_TRUE(IsAligned(arr.Array(), 128));
  for (size_t i = 0; i < 10; ++i) {
    ASSERT_TRUE(IsAligned(arr.Row(i), 128));
    ASSERT_EQ(arr.Row(i), arr.Array()[i]);
  }
}

TEST(AlignedArrayTest, CheckOverlap) {
  AlignedArray<size_t> arr(10, 7, 128);

  for (size_t i = 0; i < 10; ++i) {
    for (size_t j = 0; j < 7; ++j) {
      arr.At(i, j) = 20 * i + j;
    }
  }

  for (size_t i = 0; i < 10; ++i) {
    for (size_t j = 0; j < 7; ++j) {
      ASSERT_EQ(arr.At(i, j), 20 * i + j);
      ASSERT_EQ(arr.Row(i)[j], 20 * i + j);
      ASSERT_EQ(arr.Array()[i][j], 20 * i + j);
    }
  }
}

TEST(AlignedArrayTest, CheckRowsCols) {
  AlignedArray<bool> arr(10, 7, 128);
  ASSERT_EQ(arr.rows(), 10u);
  ASSERT_EQ(arr.cols(), 7u);
}

}  // namespace sm

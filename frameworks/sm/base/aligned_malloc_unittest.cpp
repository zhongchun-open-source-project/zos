// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/aligned_malloc.h"

#if _WIN32
#include <windows.h>
#else
#include <stdint.h>
#endif

#include "testing/gtest/include/gtest/gtest.h"
#include "sm/base/scoped_ptr.h"
#include "sm/typedefs.h"

namespace sm {

// Returns true if |size| and |alignment| are valid combinations.
bool CorrectUsage(size_t size, size_t alignment) {
  scoped_ptr<char, AlignedFreeDeleter> scoped(
      static_cast<char*>(AlignedMalloc(size, alignment)));
  if (scoped.get() == NULL) {
    return false;
  }
  const uintptr_t scoped_address = reinterpret_cast<uintptr_t> (scoped.get());
  return 0u == scoped_address % alignment;
}

TEST(AlignedMalloc, GetRightAlign) {
  const size_t size = 100;
  const size_t alignment = 32;
  const size_t left_misalignment = 1;
  scoped_ptr<char, AlignedFreeDeleter> scoped(
      static_cast<char*>(AlignedMalloc(size, alignment)));
  EXPECT_TRUE(scoped.get() != NULL);
  const uintptr_t aligned_address = reinterpret_cast<uintptr_t> (scoped.get());
  const uintptr_t misaligned_address = aligned_address - left_misalignment;
  const char* misaligned_ptr = reinterpret_cast<const char*>(
      misaligned_address);
  const char* realigned_ptr = GetRightAlign(misaligned_ptr, alignment);
  EXPECT_EQ(scoped.get(), realigned_ptr);
}

TEST(AlignedMalloc, IncorrectSize) {
  const size_t incorrect_size = 0;
  const size_t alignment = 64;
  EXPECT_FALSE(CorrectUsage(incorrect_size, alignment));
}

TEST(AlignedMalloc, IncorrectAlignment) {
  const size_t size = 100;
  const size_t incorrect_alignment = 63;
  EXPECT_FALSE(CorrectUsage(size, incorrect_alignment));
}

TEST(AlignedMalloc, AlignTo2Bytes) {
  size_t size = 100;
  size_t alignment = 2;
  EXPECT_TRUE(CorrectUsage(size, alignment));
}

TEST(AlignedMalloc, AlignTo32Bytes) {
  size_t size = 100;
  size_t alignment = 32;
  EXPECT_TRUE(CorrectUsage(size, alignment));
}

TEST(AlignedMalloc, AlignTo128Bytes) {
  size_t size = 100;
  size_t alignment = 128;
  EXPECT_TRUE(CorrectUsage(size, alignment));
}

}  // namespace sm


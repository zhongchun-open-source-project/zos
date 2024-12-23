// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/crc32.h"
#include "sm/base/gunit.h"

#include <string>

namespace sm {

TEST(Crc32Test, TestBasic) {
  EXPECT_EQ(0U, ComputeCrc32(""));
  EXPECT_EQ(0x352441C2U, ComputeCrc32("abc"));
  EXPECT_EQ(0x171A3F5FU,
      ComputeCrc32("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"));
}

TEST(Crc32Test, TestMultipleUpdates) {
  std::string input =
      "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
  uint32_t c = 0;
  for (size_t i = 0; i < input.size(); ++i) {
    c = UpdateCrc32(c, &input[i], 1);
  }
  EXPECT_EQ(0x171A3F5FU, c);
}

}  // namespace sm

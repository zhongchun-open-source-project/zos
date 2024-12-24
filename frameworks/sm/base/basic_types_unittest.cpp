// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/basic_types.h"

#include "sm/base/gunit.h"

namespace sm {

TEST(BasicTypesTest, Endian) {
  uint16_t v16 = 0x1234u;
  uint8_t first_byte = *reinterpret_cast<uint8_t*>(&v16);
#if defined(SM_ARCH_CPU_LITTLE_ENDIAN)
  EXPECT_EQ(0x34u, first_byte);
#elif defined(SM_ARCH_CPU_BIG_ENDIAN)
  EXPECT_EQ(0x12u, first_byte);
#endif
}

TEST(BasicTypesTest, SizeOfConstants) {
  EXPECT_EQ(8u, sizeof(INT64_C(0)));
  EXPECT_EQ(8u, sizeof(UINT64_C(0)));
  EXPECT_EQ(8u, sizeof(INT64_C(0x1234567887654321)));
  EXPECT_EQ(8u, sizeof(UINT64_C(0x8765432112345678)));
}

// Test CPU_ macros
#if !defined(CPU_ARM) && defined(__arm__)
#error expected CPU_ARM to be defined.
#endif
#if !defined(CPU_X86) && (defined(SM_WIN) || defined(SM_MAC) && !defined(SM_IOS))
#error expected CPU_X86 to be defined.
#endif
#if !defined(SM_ARCH_CPU_LITTLE_ENDIAN) && \
  (defined(SM_WIN) || defined(SM_MAC) && !defined(SM_IOS) || defined(CPU_X86))
#error expected SM_ARCH_CPU_LITTLE_ENDIAN to be defined.
#endif

// TODO(fbarchard): Test all macros in basictypes.h

}  // namespace sm

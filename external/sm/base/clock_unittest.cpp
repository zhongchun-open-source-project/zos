// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/clock.h"

#include "testing/gtest/include/gtest/gtest.h"

namespace sm {

TEST(ClockTest, NtpTime) {
  Clock* clock = Clock::GetRealTimeClock();
  uint32_t seconds;
  uint32_t fractions;
  clock->CurrentNtp(seconds, fractions);
  int64_t milliseconds = clock->CurrentNtpInMilliseconds();
  EXPECT_GT(milliseconds / 1000, kNtpJan1970);
  EXPECT_GE(milliseconds, Clock::NtpToMs(seconds, fractions));
  EXPECT_NEAR(milliseconds, Clock::NtpToMs(seconds, fractions), 100);
}

}  // namespace sm

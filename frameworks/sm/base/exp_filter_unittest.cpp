// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include <math.h>

#include "sm/base/gunit.h"
#include "sm/base/exp_filter.h"

namespace sm {

TEST(ExpFilterTest, FirstTimeOutputEqualInput) {
  // No max value defined.
  ExpFilter filter = ExpFilter(0.9f);
  filter.Apply(100.0f, 10.0f);

  // First time, first argument no effect.
  double value = 10.0f;
  EXPECT_FLOAT_EQ(value, filter.filtered());
}

TEST(ExpFilterTest, SecondTime) {
  double value;

  ExpFilter filter = ExpFilter(0.9f);
  filter.Apply(100.0f, 10.0f);

  // First time, first argument no effect.
  value = 10.0f;

  filter.Apply(10.0f, 20.0f);
  double alpha = pow(0.9f, 10.0f);
  value = alpha * value + (1.0f - alpha) * 20.0f;
  EXPECT_FLOAT_EQ(value, filter.filtered());
}

TEST(ExpFilterTest, Reset) {
  ExpFilter filter = ExpFilter(0.9f);
  filter.Apply(100.0f, 10.0f);

  filter.Reset(0.8f);
  filter.Apply(100.0f, 1.0f);

  // Become first time after a reset.
  double value = 1.0f;
  EXPECT_FLOAT_EQ(value, filter.filtered());
}

TEST(ExpfilterTest, OutputLimitedByMax) {
  double value;

  // Max value defined.
  ExpFilter filter = ExpFilter(0.9f, 1.0f);
  filter.Apply(100.0f, 10.0f);

  // Limited to max value.
  value = 1.0f;
  EXPECT_EQ(value, filter.filtered());

  filter.Apply(1.0f, 0.0f);
  value = 0.9f * value;
  EXPECT_FLOAT_EQ(value, filter.filtered());
}

}  // namespace sm

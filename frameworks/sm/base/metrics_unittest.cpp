// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "testing/gtest/include/gtest/gtest.h"

#include "sm/base/metrics.h"
#include "sm/test/histogram.h"

namespace sm {
namespace {
const int kSample = 22;
const std::string kName = "Name";

void AddSparseSample(const std::string& name, int sample) {
  RTC_HISTOGRAM_COUNTS_SPARSE_100(name, sample);
}
#if GTEST_HAS_DEATH_TEST && !defined(SM_ANDROID)
void AddSample(const std::string& name, int sample) {
  RTC_HISTOGRAM_COUNTS_100(name, sample);
}
#endif  // GTEST_HAS_DEATH_TEST && !defined(SM_ANDROID)
}  // namespace

TEST(MetricsTest, InitiallyNoSamples) {
  test::ClearHistograms();
  EXPECT_EQ(0, test::NumHistogramSamples(kName));
  EXPECT_EQ(-1, test::LastHistogramSample(kName));
}

TEST(MetricsTest, SmHistogramPercent_AddSample) {
  test::ClearHistograms();
  RTC_HISTOGRAM_PERCENTAGE(kName, kSample);
  EXPECT_EQ(1, test::NumHistogramSamples(kName));
  EXPECT_EQ(kSample, test::LastHistogramSample(kName));
}

TEST(MetricsTest, SmHistogramEnumeration_AddSample) {
  test::ClearHistograms();
  RTC_HISTOGRAM_ENUMERATION(kName, kSample, kSample + 1);
  EXPECT_EQ(1, test::NumHistogramSamples(kName));
  EXPECT_EQ(kSample, test::LastHistogramSample(kName));
}

TEST(MetricsTest, SmHistogramCountsSparse_AddSample) {
  test::ClearHistograms();
  RTC_HISTOGRAM_COUNTS_SPARSE_100(kName, kSample);
  EXPECT_EQ(1, test::NumHistogramSamples(kName));
  EXPECT_EQ(kSample, test::LastHistogramSample(kName));
}

TEST(MetricsTest, SmHistogramCounts_AddSample) {
  test::ClearHistograms();
  RTC_HISTOGRAM_COUNTS_100(kName, kSample);
  EXPECT_EQ(1, test::NumHistogramSamples(kName));
  EXPECT_EQ(kSample, test::LastHistogramSample(kName));
}

TEST(MetricsTest, SmHistogramCounts_AddMultipleSamples) {
  test::ClearHistograms();
  const int kNumSamples = 10;
  for (int i = 0; i < kNumSamples; ++i) {
    RTC_HISTOGRAM_COUNTS_100(kName, i);
  }
  EXPECT_EQ(kNumSamples, test::NumHistogramSamples(kName));
  EXPECT_EQ(kNumSamples - 1, test::LastHistogramSample(kName));
}

TEST(MetricsTest, SmHistogramSparse_NonConstantNameWorks) {
  test::ClearHistograms();
  AddSparseSample("Name1", kSample);
  AddSparseSample("Name2", kSample);
  EXPECT_EQ(1, test::NumHistogramSamples("Name1"));
  EXPECT_EQ(1, test::NumHistogramSamples("Name2"));
}

#if GTEST_HAS_DEATH_TEST && !defined(SM_ANDROID)
TEST(MetricsTest, SmHistogram_FailsForNonConstantName) {
  test::ClearHistograms();
  AddSample("Name1", kSample);
  EXPECT_DEATH(AddSample("Name2", kSample), "");
}
#endif  // GTEST_HAS_DEATH_TEST && !defined(SM_ANDROID)

}  // namespace sm

// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_RATE_TRACKER_H_
#define SM_BASE_RATE_TRACKER_H_

#include <stdlib.h>
#include "sm/base/basic_types.h"

namespace sm {

// Computes units per second over a given interval by tracking the units over
// each bucket of a given size and calculating the instantaneous rate assuming
// that over each bucket the rate was constant.
class RateTracker {
 public:
  RateTracker(uint32_t bucket_milliseconds, size_t bucket_count);
  virtual ~RateTracker();

  // Computes the average rate over the most recent interval_milliseconds,
  // or if the first sample was added within this period, computes the rate
  // since the first sample was added.
  double ComputeRateForInterval(uint32_t interval_milliseconds) const;

  // Computes the average rate over the rate tracker's recording interval
  // of bucket_milliseconds * bucket_count.
  double ComputeRate() const {
    return ComputeRateForInterval(bucket_milliseconds_ *
                                  static_cast<uint32_t>(bucket_count_));
  }

  // Computes the average rate since the first sample was added to the
  // rate tracker.
  double ComputeTotalRate() const;

  // The total number of samples added.
  size_t TotalSampleCount() const;

  // Reads the current time in order to determine the appropriate bucket for
  // these samples, and increments the count for that bucket by sample_count.
  void AddSamples(size_t sample_count);

 protected:
  // overrideable for tests
  virtual uint32_t Time() const;

 private:
  void EnsureInitialized();
  size_t NextBucketIndex(size_t bucket_index) const;

  const uint32_t bucket_milliseconds_;
  const size_t bucket_count_;
  size_t* sample_buckets_;
  size_t total_sample_count_;
  size_t current_bucket_;
  uint32_t bucket_start_time_milliseconds_;
  uint32_t initialization_time_milliseconds_;
};

}  // namespace sm

#endif  // SM_BASE_RATE_TRACKER_H_

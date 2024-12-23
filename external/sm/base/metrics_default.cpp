// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/metrics.h"

// Default implementation of histogram methods for WebRTC clients that do not
// want to provide their own implementation.

namespace sm {
namespace metrics {

Histogram* HistogramFactoryGetCounts(const std::string& name, int min, int max,
    int bucket_count) { return NULL; }

Histogram* HistogramFactoryGetEnumeration(const std::string& name,
    int boundary) { return NULL; }

void HistogramAdd(
    Histogram* histogram_pointer, const std::string& name, int sample) {}

}  // namespace metrics
}  // namespace sm


// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_SAFE_CONVERSIONS_H_
#define SM_BASE_SAFE_CONVERSIONS_H_

#include <limits>

#include "sm/base/checks.h"
#include "sm/base/safe_conversions_impl.h"

namespace sm {

// Convenience function that returns true if the supplied value is in range
// for the destination type.
template <typename Dst, typename Src>
inline bool IsValueInRangeForNumericType(Src value) {
  return internal::RangeCheck<Dst>(value) == internal::TYPE_VALID;
}

// checked_cast<> is analogous to static_cast<> for numeric types,
// except that it CHECKs that the specified numeric conversion will not
// overflow or underflow. NaN source will always trigger a CHECK.
template <typename Dst, typename Src>
inline Dst checked_cast(Src value) {
  SM_CHECK(IsValueInRangeForNumericType<Dst>(value));
  return static_cast<Dst>(value);
}

// saturated_cast<> is analogous to static_cast<> for numeric types, except
// that the specified numeric conversion will saturate rather than overflow or
// underflow. NaN assignment to an integral will trigger a SM_CHECK condition.
template <typename Dst, typename Src>
inline Dst saturated_cast(Src value) {
  // Optimization for floating point values, which already saturate.
  if (std::numeric_limits<Dst>::is_iec559)
    return static_cast<Dst>(value);

  switch (internal::RangeCheck<Dst>(value)) {
    case internal::TYPE_VALID:
      return static_cast<Dst>(value);

    case internal::TYPE_UNDERFLOW:
      return std::numeric_limits<Dst>::min();

    case internal::TYPE_OVERFLOW:
      return std::numeric_limits<Dst>::max();

    // Should fail only on attempting to assign NaN to a saturated integer.
    case internal::TYPE_INVALID:
      FATAL();
      return std::numeric_limits<Dst>::max();
  }

  FATAL();
  return static_cast<Dst>(value);
}

}  // namespace sm

#endif  // SM_BASE_SAFE_CONVERSIONS_H_

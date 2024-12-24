// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_RANDOM_H_
#define SM_BASE_RANDOM_H_

#include <limits>

#include "sm/typedefs.h"
#include "sm/base/constructor_magic.h"
#include "sm/base/checks.h"

namespace sm {

class Random {
 public:
  explicit Random(uint64_t seed);

  // Return pseudo-random integer of the specified type.
  // We need to limit the size to 32 bits to keep the output close to uniform.
  template <typename T>
  T Rand() {
    static_assert(std::numeric_limits<T>::is_integer &&
                      std::numeric_limits<T>::radix == 2 &&
                      std::numeric_limits<T>::digits <= 32,
                  "Rand is only supported for built-in integer types that are "
                  "32 bits or smaller.");
    return static_cast<T>(NextOutput());
  }

  // Uniformly distributed pseudo-random number in the interval [0, t].
  uint32_t Rand(uint32_t t);

  // Uniformly distributed pseudo-random number in the interval [low, high].
  uint32_t Rand(uint32_t low, uint32_t high);

  // Uniformly distributed pseudo-random number in the interval [low, high].
  int32_t Rand(int32_t low, int32_t high);

  // Normal Distribution.
  double Gaussian(double mean, double standard_deviation);

  // Exponential Distribution.
  double Exponential(double lambda);

 private:
  // Outputs a nonzero 64-bit random number.
  uint64_t NextOutput() {
    state_ ^= state_ >> 12;
    state_ ^= state_ << 25;
    state_ ^= state_ >> 27;
    SM_DCHECK(state_ != 0x0ULL);
    return state_ * 2685821657736338717ull;
  }

  uint64_t state_;

  SM_DISALLOW_IMPLICIT_CONSTRUCTORS(Random);
};

// Return pseudo-random number in the interval [0.0, 1.0).
template <>
float Random::Rand<float>();

// Return pseudo-random number in the interval [0.0, 1.0).
template <>
double Random::Rand<double>();

// Return pseudo-random boolean value.
template <>
bool Random::Rand<bool>();

}  // namespace sm

#endif  // SM_BASE_RANDOM_H_

// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/atomic32.h"

#include <assert.h>
#include <windows.h>

#include "sm/common_types.h"

namespace sm {

Atomic32::Atomic32(int32_t initial_value)
    : value_(initial_value) {
  static_assert(sizeof(value_) == sizeof(LONG),
                "counter variable is the expected size");
  assert(Is32bitAligned());
}

Atomic32::~Atomic32() {
}

int32_t Atomic32::operator++() {
  return static_cast<int32_t>(InterlockedIncrement(
      reinterpret_cast<volatile LONG*>(&value_)));
}

int32_t Atomic32::operator--() {
  return static_cast<int32_t>(InterlockedDecrement(
      reinterpret_cast<volatile LONG*>(&value_)));
}

int32_t Atomic32::operator+=(int32_t value) {
  return InterlockedExchangeAdd(reinterpret_cast<volatile LONG*>(&value_),
                                value);
}

int32_t Atomic32::operator-=(int32_t value) {
  return InterlockedExchangeAdd(reinterpret_cast<volatile LONG*>(&value_),
                                -value);
}

bool Atomic32::CompareExchange(int32_t new_value, int32_t compare_value) {
  const LONG old_value = InterlockedCompareExchange(
      reinterpret_cast<volatile LONG*>(&value_),
      new_value,
      compare_value);

  // If the old value and the compare value is the same an exchange happened.
  return (old_value == compare_value);
}

}  // namespace sm

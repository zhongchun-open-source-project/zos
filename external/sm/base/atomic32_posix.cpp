// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/atomic32.h"

#include <assert.h>
#include <inttypes.h>
#include <malloc.h>

#include "sm/common_types.h"

namespace sm {

Atomic32::Atomic32(int32_t initial_value)
    : value_(initial_value) {
  assert(Is32bitAligned());
}

Atomic32::~Atomic32() {
}

int32_t Atomic32::operator++() {
  return __sync_fetch_and_add(&value_, 1) + 1;
}

int32_t Atomic32::operator--() {
  return __sync_fetch_and_sub(&value_, 1) - 1;
}

int32_t Atomic32::operator+=(int32_t value) {
  int32_t return_value = __sync_fetch_and_add(&value_, value);
  return_value += value;
  return return_value;
}

int32_t Atomic32::operator-=(int32_t value) {
  int32_t return_value = __sync_fetch_and_sub(&value_, value);
  return_value -= value;
  return return_value;
}

bool Atomic32::CompareExchange(int32_t new_value, int32_t compare_value) {
  return __sync_bool_compare_and_swap(&value_, compare_value, new_value);
}

}  // namespace sm

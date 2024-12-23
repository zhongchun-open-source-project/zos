// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_ATOMIC_OPS_H_
#define SM_BASE_ATOMIC_OPS_H_

#if defined(SM_WIN)
// Include winsock2.h before including <windows.h> to maintain consistency with
// win32.h.  We can't include win32.h directly here since it pulls in
// headers such as basictypes.h which causes problems in Chromium where sm
// exists as two separate projects, sm and libsm.
#include <winsock2.h>
#include <windows.h>
#endif  // defined(SM_WIN)

namespace sm {
class AtomicOps {
 public:
#if defined(SM_WIN)
  // Assumes sizeof(int) == sizeof(LONG), which it is on Win32 and Win64.
  static int Increment(volatile int* i) {
    return ::InterlockedIncrement(reinterpret_cast<volatile LONG*>(i));
  }
  static int Decrement(volatile int* i) {
    return ::InterlockedDecrement(reinterpret_cast<volatile LONG*>(i));
  }
  static int AcquireLoad(volatile const int* i) {
    return *i;
  }
  static void ReleaseStore(volatile int* i, int value) {
    *i = value;
  }
  static int CompareAndSwap(volatile int* i, int old_value, int new_value) {
    return ::InterlockedCompareExchange(reinterpret_cast<volatile LONG*>(i),
                                        new_value,
                                        old_value);
  }
  // Pointer variants.
  template <typename T>
  static T* AcquireLoadPtr(T* volatile* ptr) {
    return *ptr;
  }
  template <typename T>
  static T* CompareAndSwapPtr(T* volatile* ptr, T* old_value, T* new_value) {
    return static_cast<T*>(::InterlockedCompareExchangePointer(
        reinterpret_cast<PVOID volatile*>(ptr), new_value, old_value));
  }
#else
  static int Increment(volatile int* i) {
    return __sync_add_and_fetch(i, 1);
  }
  static int Decrement(volatile int* i) {
    return __sync_sub_and_fetch(i, 1);
  }
  static int AcquireLoad(volatile const int* i) {
    return __atomic_load_n(i, __ATOMIC_ACQUIRE);
  }
  static void ReleaseStore(volatile int* i, int value) {
    __atomic_store_n(i, value, __ATOMIC_RELEASE);
  }
  static int CompareAndSwap(volatile int* i, int old_value, int new_value) {
    return __sync_val_compare_and_swap(i, old_value, new_value);
  }
  // Pointer variants.
  template <typename T>
  static T* AcquireLoadPtr(T* volatile* ptr) {
    return __atomic_load_n(ptr, __ATOMIC_ACQUIRE);
  }
  template <typename T>
  static T* CompareAndSwapPtr(T* volatile* ptr, T* old_value, T* new_value) {
    return __sync_val_compare_and_swap(ptr, old_value, new_value);
  }
#endif
};

}

#endif  // SM_BASE_ATOMIC_OPS_H_

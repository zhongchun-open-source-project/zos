// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_EVENT_H__
#define SM_BASE_EVENT_H__

#if defined(SM_WIN)
#include "sm/base/win32.h"  // NOLINT: consider this a system header.
#elif defined(SM_POSIX)
#include <pthread.h>
#else
#error "Must define either SM_WIN or SM_POSIX."
#endif

#include "sm/base/basic_types.h"

namespace sm {

class Event {
 public:
  static const int kForever = -1;

  Event(bool manual_reset, bool initially_signaled);
  ~Event();

  void Set();
  void Reset();

  // Wait for the event to become signaled, for the specified number of
  // |milliseconds|.  To wait indefinetly, pass kForever.
  bool Wait(int milliseconds);

 private:
#if defined(SM_WIN)
  HANDLE event_handle_;
#elif defined(SM_POSIX)
  pthread_mutex_t event_mutex_;
  pthread_cond_t event_cond_;
  const bool is_manual_reset_;
  bool event_status_;
#endif
};

}  // namespace sm

#endif  // SM_BASE_EVENT_H__

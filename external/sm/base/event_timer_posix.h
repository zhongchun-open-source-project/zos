// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_EVENT_TIMER_POSIX_H_
#define SM_BASE_EVENT_TIMER_POSIX_H_

#include "sm/base/event_wrapper.h"

#include <pthread.h>
#include <time.h>

#include "sm/base/platform_thread.h"

namespace sm {

enum State {
  kUp = 1,
  kDown = 2
};

class EventTimerPosix : public EventTimerWrapper {
 public:
  EventTimerPosix();
  ~EventTimerPosix() override;

  EventTypeWrapper Wait(unsigned long max_time) override;
  bool Set() override;

  bool StartTimer(bool periodic, unsigned long time) override;
  bool StopTimer() override;

 private:
  static bool Run(void* obj);
  bool Process();
  EventTypeWrapper Wait(timespec* end_at);

 private:
  pthread_cond_t  cond_;
  pthread_mutex_t mutex_;
  bool event_set_;

  // TODO(pbos): Remove scoped_ptr and use PlatformThread directly.
  scoped_ptr<PlatformThread> timer_thread_;
  scoped_ptr<EventTimerPosix> timer_event_;
  timespec       created_at_;

  bool          periodic_;
  unsigned long time_;  // In ms
  unsigned long count_;
};

}  // namespace sm

#endif  // SM_BASE_EVENT_TIMER_POSIX_H_

// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_EVENT_TIMER_WIN_H_
#define SM_BASE_EVENT_TIMER_WIN_H_

#include <windows.h>

#include "sm/base/event_wrapper.h"

#include "sm/typedefs.h"

namespace sm {

class EventTimerWin : public EventTimerWrapper {
 public:
  EventTimerWin();
  virtual ~EventTimerWin();

  virtual EventTypeWrapper Wait(unsigned long max_time);
  virtual bool Set();

  virtual bool StartTimer(bool periodic, unsigned long time);
  virtual bool StopTimer();

 private:
  HANDLE  event_;
  uint32_t timerID_;
};

}  // namespace sm

#endif  // SM_BASE_EVENT_TIMER_WIN_H_

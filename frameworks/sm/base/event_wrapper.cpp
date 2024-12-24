// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/event_wrapper.h"

#if defined(_WIN32)
#include <windows.h>
#include "sm/base/event_timer_win.h"
#elif defined(SM_MAC) && !defined(SM_IOS)
#include <ApplicationServices/ApplicationServices.h>
#include <pthread.h>
#include "sm/base/event_timer_posix.h"
#else
#include <pthread.h>
#include "sm/base/event_timer_posix.h"
#endif

#include "sm/base/event.h"

namespace sm {

class EventWrapperImpl : public EventWrapper {
 public:
  EventWrapperImpl() : event_(false, false) {}
  ~EventWrapperImpl() override {}

  bool Set() override {
    event_.Set();
    return true;
  }

  EventTypeWrapper Wait(unsigned long max_time) override {
    int to_wait = max_time == SM_EVENT_INFINITE ?
        Event::kForever : static_cast<int>(max_time);
    return event_.Wait(to_wait) ? kEventSignaled : kEventTimeout;
  }

 private:
  Event event_;
};

// static
EventWrapper* EventWrapper::Create() {
  return new EventWrapperImpl();
}

}  // namespace sm

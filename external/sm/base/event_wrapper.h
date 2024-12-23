// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_EVENT_WRAPPER_H_
#define SM_BASE_EVENT_WRAPPER_H_

namespace sm {
enum EventTypeWrapper {
  kEventSignaled = 1,
  kEventError = 2,
  kEventTimeout = 3
};

#define SM_EVENT_INFINITE 0xffffffff

class EventTimerWrapper;

class EventWrapper {
 public:
  // Factory method. Constructor disabled.
  static EventWrapper* Create();

  virtual ~EventWrapper() {}

  // Releases threads who are calling Wait() and has started waiting. Please
  // note that a thread calling Wait() will not start waiting immediately.
  // assumptions to the contrary is a very common source of issues in
  // multithreaded programming.
  // Set is sticky in the sense that it will release at least one thread
  // either immediately or some time in the future.
  virtual bool Set() = 0;

  // Puts the calling thread into a wait state. The thread may be released
  // by a Set() call depending on if other threads are waiting and if so on
  // timing. The thread that was released will reset the event before leaving
  // preventing more threads from being released. If multiple threads
  // are waiting for the same Set(), only one (random) thread is guaranteed to
  // be released. It is possible that multiple (random) threads are released
  // Depending on timing.
  //
  // |max_time| is the maximum time to wait in milliseconds or
  // SM_EVENT_INFINITE to wait infinitely.
  virtual EventTypeWrapper Wait(unsigned long max_time) = 0;
};

class EventTimerWrapper : public EventWrapper {
 public:
  static EventTimerWrapper* Create();

  // Starts a timer that will call a non-sticky version of Set() either once
  // or periodically. If the timer is periodic it ensures that there is no
  // drift over time relative to the system clock.
  //
  // |time| is in milliseconds.
  virtual bool StartTimer(bool periodic, unsigned long time) = 0;

  virtual bool StopTimer() = 0;

};

}  // namespace sm

#endif  // SM_BASE_EVENT_WRAPPER_H_

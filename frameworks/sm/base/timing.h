// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_TIMING_H_
#define SM_BASE_TIMING_H_

#if defined(SM_WIN)
#include "sm/base/win32.h"
#endif

namespace sm {

class Timing {
 public:
  Timing();
  virtual ~Timing();

  // WallTimeNow() returns the current wall-clock time in seconds,
  // within 10 milliseconds resolution.
  // WallTimeNow is static and does not require a timer_handle_ on Windows.
  static double WallTimeNow();

  // TimerNow() is like WallTimeNow(), but is monotonically
  // increasing.  It returns seconds in resolution of 10 microseconds
  // or better.  Although timer and wall-clock time have the same
  // timing unit, they do not necessarily correlate because wall-clock
  // time may be adjusted backwards, hence not monotonic.
  // Made virtual so we can make a fake one.
  virtual double TimerNow();

  // BusyWait() exhausts CPU as long as the time elapsed is less than
  // the specified interval in seconds.  Returns the actual waiting
  // time based on TimerNow() measurement.
  double BusyWait(double period);

  // IdleWait() relinquishes control of CPU for specified period in
  // seconds.  It uses highest resolution sleep mechanism as possible,
  // but does not otherwise guarantee the accuracy.  Returns the
  // actual waiting time based on TimerNow() measurement.
  //
  // This function is not re-entrant for an object.  Create a fresh
  // Timing object for each thread.
  double IdleWait(double period);

 private:
#if defined(SM_WIN)
  HANDLE timer_handle_;
#endif
};

}  // namespace sm

#endif // SM_BASE_TIMING_H_

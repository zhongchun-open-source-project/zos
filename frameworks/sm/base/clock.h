// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_CLOCK_H_
#define SM_BASE_CLOCK_H_

#include "sm/base/scoped_ptr.h"
#include "sm/base/rw_lock_wrapper.h"
#include "sm/typedefs.h"

namespace sm {

// January 1970, in NTP seconds.
const uint32_t kNtpJan1970 = 2208988800UL;

// Magic NTP fractional unit.
const double kMagicNtpFractionalUnit = 4.294967296E+9;

// A clock interface that allows reading of absolute and relative timestamps.
class Clock {
 public:
  virtual ~Clock() {}

  // Return a timestamp in milliseconds relative to some arbitrary source; the
  // source is fixed for this clock.
  virtual int64_t TimeInMilliseconds() const = 0;

  // Return a timestamp in microseconds relative to some arbitrary source; the
  // source is fixed for this clock.
  virtual int64_t TimeInMicroseconds() const = 0;

  // Retrieve an NTP absolute timestamp in seconds and fractions of a second.
  virtual void CurrentNtp(uint32_t& seconds, uint32_t& fractions) const = 0;

  // Retrieve an NTP absolute timestamp in milliseconds.
  virtual int64_t CurrentNtpInMilliseconds() const = 0;

  // Converts an NTP timestamp to a millisecond timestamp.
  static int64_t NtpToMs(uint32_t seconds, uint32_t fractions);

  // Returns an instance of the real-time system clock implementation.
  static Clock* GetRealTimeClock();
};

class SimulatedClock : public Clock {
 public:
  explicit SimulatedClock(int64_t initial_time_us);

  ~SimulatedClock() override;

  // Return a timestamp in milliseconds relative to some arbitrary source; the
  // source is fixed for this clock.
  int64_t TimeInMilliseconds() const override;

  // Return a timestamp in microseconds relative to some arbitrary source; the
  // source is fixed for this clock.
  int64_t TimeInMicroseconds() const override;

  // Retrieve an NTP absolute timestamp in milliseconds.
  void CurrentNtp(uint32_t& seconds, uint32_t& fractions) const override;

  // Converts an NTP timestamp to a millisecond timestamp.
  int64_t CurrentNtpInMilliseconds() const override;

  // Advance the simulated clock with a given number of milliseconds or
  // microseconds.
  void AdvanceTimeMilliseconds(int64_t milliseconds);
  void AdvanceTimeMicroseconds(int64_t microseconds);

 private:
  int64_t time_us_;
  scoped_ptr<RWLockWrapper> lock_;
};

};  // namespace sm

#endif  // SM_BASE_CLOCK_H_

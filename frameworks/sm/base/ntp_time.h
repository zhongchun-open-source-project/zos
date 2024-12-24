// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_NTP_TIME_H_
#define SM_BASE_NTP_TIME_H_

#include "sm/base/basic_types.h"
#include "sm/base/clock.h"

namespace sm {

class NtpTime {
 public:
  NtpTime() : seconds_(0), fractions_(0) {}
  explicit NtpTime(const Clock& clock) {
    clock.CurrentNtp(seconds_, fractions_);
  }
  NtpTime(uint32_t seconds, uint32_t fractions)
      : seconds_(seconds), fractions_(fractions) {}

  NtpTime(const NtpTime&) = default;
  NtpTime& operator=(const NtpTime&) = default;

  void SetCurrent(const Clock& clock) {
    clock.CurrentNtp(seconds_, fractions_);
  }
  void Set(uint32_t seconds, uint32_t fractions) {
    seconds_ = seconds;
    fractions_ = fractions;
  }
  void Reset() {
    seconds_ = 0;
    fractions_ = 0;
  }

  int64_t ToMs() const { return Clock::NtpToMs(seconds_, fractions_); }

  // NTP standard (RFC1305, section 3.1) explicitly state value 0/0 is invalid.
  bool Valid() const { return !(seconds_ == 0 && fractions_ == 0); }

  uint32_t seconds() const { return seconds_; }
  uint32_t fractions() const { return fractions_; }

 private:
  uint32_t seconds_;
  uint32_t fractions_;
};

inline bool operator==(const NtpTime& n1, const NtpTime& n2) {
  return n1.seconds() == n2.seconds() && n1.fractions() == n2.fractions();
}
inline bool operator!=(const NtpTime& n1, const NtpTime& n2) {
  return !(n1 == n2);
}

}  // namespace sm
#endif  // SM_BASE_NTP_TIME_H_

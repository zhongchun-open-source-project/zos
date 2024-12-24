// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_CRITICAL_SECTION_WRAPPER_H_
#define SM_BASE_CRITICAL_SECTION_WRAPPER_H_

// If the critical section is heavily contended it may be beneficial to use
// read/write locks instead.

#include "sm/base/thread_annotations.h"
#include "sm/common_types.h"

namespace sm {
class LOCKABLE CriticalSectionWrapper {
 public:
  // Factory method, constructor disabled
  static CriticalSectionWrapper* CreateCriticalSection();

  virtual ~CriticalSectionWrapper() {}

  // Tries to grab lock, beginning of a critical section. Will wait for the
  // lock to become available if the grab failed.
  virtual void Enter() EXCLUSIVE_LOCK_FUNCTION() = 0;

  // Returns a grabbed lock, end of critical section.
  virtual void Leave() UNLOCK_FUNCTION() = 0;
};

// RAII extension of the critical section. Prevents Enter/Leave mismatches and
// provides more compact critical section syntax.
class SCOPED_LOCKABLE CriticalSectionScoped {
 public:
  explicit CriticalSectionScoped(CriticalSectionWrapper* critsec)
      EXCLUSIVE_LOCK_FUNCTION(critsec)
      : ptr_crit_sec_(critsec) {
    ptr_crit_sec_->Enter();
  }

  ~CriticalSectionScoped() UNLOCK_FUNCTION() { ptr_crit_sec_->Leave(); }

 private:
  CriticalSectionWrapper* ptr_crit_sec_;
};

}  // namespace sm

#endif  // SM_BASE_CRITICAL_SECTION_WRAPPER_H_

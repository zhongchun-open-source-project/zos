// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

// General note: return values for the various pthread synchronization APIs
// are explicitly ignored here. In Chromium, the same thing is done for release.
// However, in debugging, failure in these APIs are logged.
// TODO(henrike): add logging when pthread synchronization APIs are failing.

#include "sm/base/critical_section_posix.h"

namespace sm {

CriticalSectionPosix::CriticalSectionPosix() {
  pthread_mutexattr_t attr;
  (void) pthread_mutexattr_init(&attr);
  (void) pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  (void) pthread_mutex_init(&mutex_, &attr);
}

CriticalSectionPosix::~CriticalSectionPosix() {
  (void) pthread_mutex_destroy(&mutex_);
}

void
CriticalSectionPosix::Enter() {
  (void) pthread_mutex_lock(&mutex_);
}

void
CriticalSectionPosix::Leave() {
  (void) pthread_mutex_unlock(&mutex_);
}

}  // namespace sm

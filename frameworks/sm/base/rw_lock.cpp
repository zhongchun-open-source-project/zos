// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/rw_lock_wrapper.h"

#include <assert.h>

#if defined(_WIN32)
#include "sm/base/rw_lock_generic.h"
#include "sm/base/rw_lock_win.h"
#else
#include "sm/base/rw_lock_posix.h"
#endif

namespace sm {

RWLockWrapper* RWLockWrapper::CreateRWLock() {
#ifdef _WIN32
  // Native implementation is faster, so use that if available.
  RWLockWrapper* lock = RWLockWin::Create();
  if (lock) {
    return lock;
  }
  return new RWLockGeneric();
#else
  return RWLockPosix::Create();
#endif
}

}  // namespace sm

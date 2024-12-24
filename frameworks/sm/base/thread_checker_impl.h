// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

// Borrowed from Chromium's src/base/threading/thread_checker_impl.h.

#ifndef SM_BASE_THREAD_CHECKER_IMPL_H_
#define SM_BASE_THREAD_CHECKER_IMPL_H_

#include "sm/base/critical_section.h"
#include "sm/base/platform_thread_types.h"

namespace sm {

// Real implementation of ThreadChecker, for use in debug mode, or
// for temporary use in release mode (e.g. to SM_CHECK on a threading issue
// seen only in the wild).
//
// Note: You should almost always use the ThreadChecker class to get the
// right version for your build configuration.
class ThreadCheckerImpl {
 public:
  ThreadCheckerImpl();
  ~ThreadCheckerImpl();

  bool CalledOnValidThread() const;

  // Changes the thread that is checked for in CalledOnValidThread.  This may
  // be useful when an object may be created on one thread and then used
  // exclusively on another thread.
  void DetachFromThread();

 private:
  mutable CriticalSection lock_;
  // This is mutable so that CalledOnValidThread can set it.
  // It's guarded by |lock_|.
  mutable PlatformThreadRef valid_thread_;
};

}  // namespace sm

#endif  // SM_BASE_THREAD_CHECKER_IMPL_H_

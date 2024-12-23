// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

// Borrowed from Chromium's src/base/threading/thread_checker_impl.cc.

#include "sm/base/thread_checker_impl.h"

#include "sm/base/platform_thread.h"

namespace sm {

ThreadCheckerImpl::ThreadCheckerImpl() : valid_thread_(CurrentThreadRef()) {
}

ThreadCheckerImpl::~ThreadCheckerImpl() {
}

bool ThreadCheckerImpl::CalledOnValidThread() const {
  const PlatformThreadRef current_thread = CurrentThreadRef();
  CritScope scoped_lock(&lock_);
  if (!valid_thread_)  // Set if previously detached.
    valid_thread_ = current_thread;
  return IsThreadRefEqual(valid_thread_, current_thread);
}

void ThreadCheckerImpl::DetachFromThread() {
  CritScope scoped_lock(&lock_);
  valid_thread_ = 0;
}

}  // namespace sm

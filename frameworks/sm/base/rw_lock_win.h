// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_RW_LOCK_WIN_H_
#define SM_BASE_RW_LOCK_WIN_H_

#include "sm/base/rw_lock_wrapper.h"

#include <Windows.h>

namespace sm {

class RWLockWin : public RWLockWrapper {
 public:
  static RWLockWin* Create();
  ~RWLockWin() {}

  virtual void AcquireLockExclusive();
  virtual void ReleaseLockExclusive();

  virtual void AcquireLockShared();
  virtual void ReleaseLockShared();

 private:
  RWLockWin();
  static bool LoadModule();

  SRWLOCK lock_;
};

}  // namespace sm

#endif  // SM_BASE_RW_LOCK_WIN_H_

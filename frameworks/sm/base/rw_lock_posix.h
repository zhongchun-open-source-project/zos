// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_RW_LOCK_POSIX_H_
#define SM_BASE_RW_LOCK_POSIX_H_

#include "sm/base/rw_lock_wrapper.h"
#include "sm/typedefs.h"

#include <pthread.h>

namespace sm {

class RWLockPosix : public RWLockWrapper {
 public:
  static RWLockPosix* Create();
  ~RWLockPosix() override;

  void AcquireLockExclusive() override;
  void ReleaseLockExclusive() override;

  void AcquireLockShared() override;
  void ReleaseLockShared() override;

 private:
  RWLockPosix();
  bool Init();

  pthread_rwlock_t lock_;
};

}  // namespace sm

#endif  // SM_BASE_RW_LOCK_POSIX_H_

// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_SHARED_EXCLUSIVE_LOCK_H_
#define SM_BASE_SHARED_EXCLUSIVE_LOCK_H_

#include "sm/base/constructor_magic.h"
#include "sm/base/critical_section.h"
#include "sm/base/event.h"

namespace sm {

// This class provides shared-exclusive lock. It can be used in cases like
// multiple-readers/single-writer model.
class LOCKABLE SharedExclusiveLock {
 public:
  SharedExclusiveLock();

  // Locking/unlocking methods. It is encouraged to use SharedScope or
  // ExclusiveScope for protection.
  void LockExclusive() EXCLUSIVE_LOCK_FUNCTION();
  void UnlockExclusive() UNLOCK_FUNCTION();
  void LockShared();
  void UnlockShared();

 private:
  CriticalSection cs_exclusive_;
  CriticalSection cs_shared_;
  Event shared_count_is_zero_;
  int shared_count_;

  SM_DISALLOW_COPY_AND_ASSIGN(SharedExclusiveLock);
};

class SCOPED_LOCKABLE SharedScope {
 public:
  explicit SharedScope(SharedExclusiveLock* lock) SHARED_LOCK_FUNCTION(lock)
      : lock_(lock) {
    lock_->LockShared();
  }

  ~SharedScope() UNLOCK_FUNCTION() { lock_->UnlockShared(); }

 private:
  SharedExclusiveLock* lock_;

  SM_DISALLOW_COPY_AND_ASSIGN(SharedScope);
};

class SCOPED_LOCKABLE ExclusiveScope {
 public:
  explicit ExclusiveScope(SharedExclusiveLock* lock)
      EXCLUSIVE_LOCK_FUNCTION(lock)
      : lock_(lock) {
    lock_->LockExclusive();
  }

  ~ExclusiveScope() UNLOCK_FUNCTION() { lock_->UnlockExclusive(); }

 private:
  SharedExclusiveLock* lock_;

  SM_DISALLOW_COPY_AND_ASSIGN(ExclusiveScope);
};

}  // namespace sm

#endif  // SM_BASE_SHARED_EXCLUSIVE_LOCK_H_

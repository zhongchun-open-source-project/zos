// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_RW_LOCK_GENERIC_H_
#define SM_BASE_RW_LOCK_GENERIC_H_

#include "sm/base/rw_lock_wrapper.h"
#include "sm/typedefs.h"

namespace sm {

class CriticalSectionWrapper;
class ConditionVariableWrapper;

class RWLockGeneric : public RWLockWrapper {
 public:
  RWLockGeneric();
  ~RWLockGeneric() override;

  void AcquireLockExclusive() override;
  void ReleaseLockExclusive() override;

  void AcquireLockShared() override;
  void ReleaseLockShared() override;

 private:
  CriticalSectionWrapper* critical_section_;
  ConditionVariableWrapper* read_condition_;
  ConditionVariableWrapper* write_condition_;

  int readers_active_;
  bool writer_active_;
  int readers_waiting_;
  int writers_waiting_;
};

}  // namespace sm

#endif  // SM_BASE_RW_LOCK_GENERIC_H_

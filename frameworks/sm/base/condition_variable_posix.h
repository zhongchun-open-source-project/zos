// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_CONDITION_VARIABLE_POSIX_H_
#define SM_BASE_CONDITION_VARIABLE_POSIX_H_

#include <pthread.h>

#include "sm/base/condition_variable_wrapper.h"
#include "sm/typedefs.h"

namespace sm {

class ConditionVariablePosix : public ConditionVariableWrapper {
 public:
  static ConditionVariableWrapper* Create();
  ~ConditionVariablePosix() override;

  void SleepCS(CriticalSectionWrapper& crit_sect) override;
  bool SleepCS(CriticalSectionWrapper& crit_sect,
               unsigned long max_time_in_ms) override;
  void Wake() override;
  void WakeAll() override;

 private:
  ConditionVariablePosix();
  int Construct();

 private:
  pthread_cond_t cond_;
};

}  // namespace sm

#endif  // SM_BASE_CONDITION_VARIABLE_POSIX_H_

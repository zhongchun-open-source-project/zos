// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_CRITICAL_SECTION_POSIX_H_
#define SM_BASE_CRITICAL_SECTION_POSIX_H_

#include "sm/base/critical_section_wrapper.h"

#include <pthread.h>

namespace sm {

class CriticalSectionPosix : public CriticalSectionWrapper {
 public:
  CriticalSectionPosix();

  ~CriticalSectionPosix() override;

  void Enter() override;
  void Leave() override;

 private:
  pthread_mutex_t mutex_;
  friend class ConditionVariablePosix;
};

}  // namespace sm

#endif  // SM_BASE_CRITICAL_SECTION_POSIX_H_

// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_CRITICAL_SECTION_WIN_H_
#define SM_BASE_CRITICAL_SECTION_WIN_H_

#include <windows.h>
#include "sm/base/critical_section_wrapper.h"
#include "sm/typedefs.h"

namespace sm {

class CriticalSectionWindows : public CriticalSectionWrapper {
 public:
  CriticalSectionWindows();

  virtual ~CriticalSectionWindows();

  virtual void Enter();
  virtual void Leave();

 private:
  CRITICAL_SECTION crit;

  friend class ConditionVariableEventWin;
  friend class ConditionVariableNativeWin;
};

}  // namespace sm

#endif  // SM_BASE_CRITICAL_SECTION_WIN_H_

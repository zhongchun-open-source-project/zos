// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_CONDITION_VARIABLE_NATIVE_WIN_H_
#define SM_BASE_CONDITION_VARIABLE_NATIVE_WIN_H_

#include <windows.h>

#include "sm/base/condition_variable_wrapper.h"

namespace sm {

#if !defined CONDITION_VARIABLE_INIT
typedef struct RTL_CONDITION_VARIABLE_ {
  void* Ptr;
} RTL_CONDITION_VARIABLE, *PRTL_CONDITION_VARIABLE;

typedef RTL_CONDITION_VARIABLE CONDITION_VARIABLE, *PCONDITION_VARIABLE;
#endif

typedef void (WINAPI* PInitializeConditionVariable)(PCONDITION_VARIABLE);
typedef BOOL (WINAPI* PSleepConditionVariableCS)(PCONDITION_VARIABLE,
                                                 PCRITICAL_SECTION, DWORD);
typedef void (WINAPI* PWakeConditionVariable)(PCONDITION_VARIABLE);
typedef void (WINAPI* PWakeAllConditionVariable)(PCONDITION_VARIABLE);

class ConditionVariableNativeWin : public ConditionVariableWrapper {
 public:
  static ConditionVariableWrapper* Create();
  virtual ~ConditionVariableNativeWin();

  void SleepCS(CriticalSectionWrapper& crit_sect);
  bool SleepCS(CriticalSectionWrapper& crit_sect, unsigned long max_time_inMS);
  void Wake();
  void WakeAll();

 private:
  ConditionVariableNativeWin();

  bool Init();

  CONDITION_VARIABLE condition_variable_;
};

}  // namespace sm

#endif  // SM_BASE_CONDITION_VARIABLE_NATIVE_WIN_H_

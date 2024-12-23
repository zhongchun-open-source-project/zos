// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include "sm/base/condition_variable_wrapper.h"

#if defined(_WIN32)
#include <windows.h>
#include "sm/base/condition_variable_event_win.h"
#include "sm/base/condition_variable_native_win.h"
#elif defined(SM_LINUX) || defined(SM_MAC)
#include <pthread.h>
#include "sm/base/condition_variable_posix.h"
#endif

namespace sm {

ConditionVariableWrapper* ConditionVariableWrapper::CreateConditionVariable() {
#if defined(_WIN32)
  // Try to create native condition variable implementation.
  ConditionVariableWrapper* ret_val = ConditionVariableNativeWin::Create();
  if (!ret_val) {
    // Native condition variable implementation does not exist. Create generic
    // condition variable based on events.
    ret_val = new ConditionVariableEventWin();
  }
  return ret_val;
#elif defined(SM_LINUX) || defined(SM_MAC)
  return ConditionVariablePosix::Create();
#else
  return NULL;
#endif
}

}  // namespace sm

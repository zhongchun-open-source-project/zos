// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if SM_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif  // SM_WIN 

#if defined(SM_MAC) && !defined(SM_IOS)
#include <CoreServices/CoreServices.h>
#endif  // SM_MAC && !defined(SM_IOS)

#include <algorithm>
#include "sm/base/common.h"
#include "sm/base/logging.h"

//////////////////////////////////////////////////////////////////////
// Assertions
//////////////////////////////////////////////////////////////////////

namespace sm {

void Break() {
#if SM_WIN
  ::DebugBreak();
#else  // !SM_WIN 
  // On POSIX systems, SIGTRAP signals debuggers to break without killing the
  // process. If a debugger isn't attached, the uncaught SIGTRAP will crash the
  // app.
  raise(SIGTRAP);
#endif
  // If a debugger wasn't attached, we will have crashed by this point. If a
  // debugger is attached, we'll continue from here.
}

static AssertLogger custom_assert_logger_ = NULL;

void SetCustomAssertLogger(AssertLogger logger) {
  custom_assert_logger_ = logger;
}

void LogAssert(const char* function, const char* file, int line,
               const char* expression) {
  if (custom_assert_logger_) {
    custom_assert_logger_(function, file, line, expression);
  } else {
    LOG(LS_ERROR) << file << "(" << line << ")" << ": ASSERT FAILED: "
                  << expression << " @ " << function;
  }
}

bool IsOdd(int n) {
  return (n & 0x1);
}

bool IsEven(int n) {
  return !IsOdd(n);
}

} // namespace sm

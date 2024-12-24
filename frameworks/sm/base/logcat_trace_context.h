// Copyright (c) 2023 Beijing Zhongchun Technology Co., Ltd. All Rights Reserved.

#ifndef SM_BASE_LOGCAT_TRACE_CONTEXT_H_
#define SM_BASE_LOGCAT_TRACE_CONTEXT_H_

#include "sm/base/trace.h"

#ifndef ANDROID
#error This file only makes sense to include on Android!
#endif

namespace sm {

// Scoped helper class for directing Traces to Android's logcat facility.  While
// this object lives, Trace output will be sent to logcat.
class LogcatTraceContext : public TraceCallback {
 public:
  LogcatTraceContext();
  virtual ~LogcatTraceContext();

  // TraceCallback impl.
  virtual void Print(TraceLevel level, const char* message, int length);
};

}  // namespace sm

#endif  // SM_BASE_LOGCAT_TRACE_CONTEXT_H_
